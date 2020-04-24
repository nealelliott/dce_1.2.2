/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volstub.c,v $
 * Revision 1.1.71.1  1996/10/02  16:59:09  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:06  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1995 Transarc Corporation
 * All Rights Reserved
 */
/*
 * volstub.c -- Evaluate Fileset families
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bak/volstub.c,v 1.1.71.1 1996/10/02 16:59:09 damon Exp $")

#include <dcedfs/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <com_err.h>
#include <dcedfs/flserver.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <bc.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

/*
 * Figure out the string corresponding to a numeric volume type, and vice versa.
 */
#define	volTypeToString(type) \
    (type == RWVOL   ? "RW" : \
     type == ROVOL   ? "RO" : \
     type == BACKVOL ? "BU" : "**Unknown**")

#define volStringToType(str) \
    ((strcmp((str), "RW") == 0) ? RWVOL : \
     (strcmp((str), "RO") == 0) ? ROVOL : \
     (strcmp((str), "BU") == 0) ? BACKVOL : -1)

static char mn[] = "volstub";	/*Module name*/
extern char *whoami;

/*-----------------------------------------------------------------------------
 * LongToDottedIP
 *
 * Description:
 *	Given a longword contaning an IP address, place the dotted IP address
 *	equivalent in the given buffer.
 *
 * Arguments:
 *	aaddr   : IP address in its longword form.
 *	adotted : Char buffer to put the dotted version in.
 *
 * Returns:
 *	0 if everything went well, or
 *	1 if we were given a null output buffer.
 *
 * Environment:
 *	The output buffer should be more than 16 characters long.
 *
 * Side Effects:
 *	None.
 *----------------------------------------------------------------------------
 */

static int LongToDottedIP(aaddr, adotted)
    long  aaddr;
    char *adotted;

{ /*LongToDottedIP*/

    char *addrbytes;	/*Bytes in the IP address*/

    /*
     * If we don't have a buffer to put things in, we go home right away.
     */
    if (adotted == (char *)0)
	return(1);

    /*
     * Map the 4 bytes to the longword, then just write them into the buffer in the
     * correct order.
     */
    addrbytes = (char *)(&aaddr);
    sprintf(adotted, "%d.%d.%d.%d",
	     (int) *addrbytes,
	     (int) *(addrbytes + 1),
	     (int) *(addrbytes + 2),
	     (int) *(addrbytes + 3));

    /*
     * Return our success.
     */
    return(0);

} /*LongToDottedIP*/

/*-----------------------------------------------------------------------------
 * bc_EvalVolumeSet
 *
 * Description:
 *	
 *
 * Arguments:
 *	aconfig	: Global configuration info.
 *	avs	: volume set name.
 *	avols	: Ptr to linked list of entries describing volumes to dump.
 *	uclient	: Ptr to Ubik client structure.
 *
 * Returns:
 *	0 on successful volume set evaluation,
 *	Lower-level codes otherwise.
 *
 * Environment:
 *	Expand only the single volume set provided (avs); don't go down its chain.
 *
 * Side Effects:
 *	None.
 *-----------------------------------------------------------------------------
 */

int bc_EvalVolumeSet(aconfig, avs, avols, uclient)
    struct bc_config *aconfig;
    struct bc_volumeSet *avs;
    struct bc_volumeDump **avols;
    struct ubik_client *uclient;

{ /*bc_EvalVolumeSet*/

  static char	rn[] = "bc_EvalVolumeSet"; /*Routine name*/
  register long code;		/*Result of various calls*/
  struct bc_volumeDump *tvd;	/*Ptr to new dump instance*/
  struct sockaddr_in sa;
  struct bc_volumeEntry *tve;	/*Ptr to new volume entry instance*/
  char patt[256];		/*Composite regex; also, target string*/
  int volType = 0;			/*Type of volume that worked */
  char srvstring[64];		/*Server portion of pattern*/
  char partstring[16];		/*Partition portion of pattern*/
  static char	anything[] = ".*"; /*Matches anything*/
  unsigned long index;		/*Current VLDB entry index*/
  unsigned long count;		/*Needed by VL_ListEntry()*/
  unsigned long next_index;	/*Next index to list*/
  struct vldbentry entry;	/*VLDB entry*/
  int	srvpartpair;		/*Loop counter: server/partition pair*/
  int matchedFlag;
  struct sockaddr_in *sin;

  long total = 0;
  int xl[MAXTYPES];

  /* Make sure we start off returning a null list of individual volumes for
   * the expansion.
   */

  *avols = (struct bc_volumeDump *) 0;

  tve = avs->ventries;
  while (tve) 
    {
      code = ubik_Call(VL_GetEntryByName, 
		       uclient,
		       0,
		       tve->name,
		       &entry);

      if (code)	{		/* cant find this, maybe regular expression */
	goto regexp_check;
      };

      tvd = (struct bc_volumeDump *) osi_Alloc(sizeof(struct bc_volumeDump));
      if (tvd == (struct bc_volumeDump *)NULL)
	return(BC_NOMEM);
      bzero((char *)tvd, sizeof(struct bc_volumeDump));
	  
      tvd->name = (char *) osi_Alloc(BU_MAXNAMELEN+1);
      if (tvd->name == (char *)NULL)
	return (BC_NOMEM);

      strcpy(tvd->name, tve->name);

      volType = RWVOL;
      if (strstr(tvd->name, ".backup") != (char *)NULL) {
	volType = BACKVOL;
      }

      if (strstr(tvd->name, ".readonly") != (char *)NULL) {
	volType = ROVOL;
      }
	
      tvd->vid = entry.VolIDs[volType];
      tvd->next = *avols;
      tvd->entry = tve;
      tvd->partition = entry.sitePartition[0];
      tvd->server = entry.siteAddr[0];
      *avols = tvd;
      tve = tve->next;
      total++;
    }			   

  dce_svc_printf(BAK_S_TOTAL_NUM_FILESETS_MSG,total);
  return(0);
  
 regexp_check:

  /* Free up any memory that may have been allocated earlier */

  tvd = *avols;
 
  while (tvd) {
    *avols  = tvd->next;
    (void) osi_Free(tvd->name, BU_MAXNAMELEN + 1 );
    (void) osi_Free(tvd, sizeof (struct bc_volumeDump));
    tvd = *avols;
  };

  *avols = 0;
    total = 0;
  index = 0;
  while ( 1 )
    {				/*w*/
	
      /* Iterate through the VLDB, printing out each entry by index.  
       * Variable next_index is set to the index of the next VLDB entry
       * in the enumeration.
       */
	
      bzero((char *)&entry, sizeof(entry));
      code = ubik_Call(VL_ListEntry, /*Routine to invoke*/
		       uclient,	/*Ubik client structure*/
		       0,	/*Ubik flags*/
		       index,	/*Current index*/
		       &count,	/*Ptr to working variable*/
		       &next_index, /*Ptr to next index value to list*/
		       &entry);	/*Ptr to entry to fill*/
      if (code) 
	return code;
	
      /* If the next index is invalid, bail out now. */
	
      if (next_index == 0 )
	break;

      matchedFlag = 0;
	
      /* Sweep through the list of volume specs in the volume set, expanding
       * each one in turn.
       */
      tve = avs->ventries;
      while (tve) 
	{
	  /* Expand current volume entry by generating the composite pattern
	   * and training the regex stuff to recognize it.
	   */
	    
	  /* compute server name based on wild-card or netaddress */
	  *((afsNetAddr *) &sa) = tve->server;
	  if (strcmp(tve->serverName, anything)==0)
	    strcpy(srvstring, anything);
	  else
	    sprintf(srvstring, "0x%lx", sa.sin_addr.s_addr);
	  /* compute parition pattern based on wild-card or partition # */
	  if (strcmp(tve->partname, anything) == 0)
	    strcpy(partstring, anything);
	  else
	    sprintf(partstring, "%d", tve->partition);
	    
	  sprintf(patt, "%s#%s#%s$",srvstring, partstring,tve->name);

	  code = re_comp(patt);
	  if (code) 
	    {
	      dce_svc_printf(BAK_S_REGEXPR_ERROR_MSG , mn, rn, patt, code);
	      return(code);
	    }
	    
	  /* We got an entry, now concatentate the pieces and try to match 
	   * it against the compiled pattern.
	   */
	    
	  /* for each server specified in the entry */
	  for (srvpartpair=0; srvpartpair < entry.nServers; srvpartpair++) 
	    {
	      sin = (struct sockaddr_in *) &entry.siteAddr[srvpartpair];
	      sprintf(patt, "0x%lx#%d#%s", sin->sin_addr.s_addr,
		      entry.sitePartition[srvpartpair],entry.name);
		
	      code = re_exec(patt);

	      fts_GetIndices( &entry, xl );
	      if ( code == 1 && 
		  xl[RWVOL] >= 0 && 
		  ( entry.siteFlags[srvpartpair] & 
		   ((unsigned)VLSF_ZEROIXHERE >> xl[RWVOL])))
		  
		  volType = RWVOL;
	      else
	      { /*e*/
		  sin = (struct sockaddr_in *) &entry.siteAddr[srvpartpair];
		  sprintf(patt, "0x%lx#%d#%s.backup",
			  sin->sin_addr.s_addr,
			  entry.sitePartition[srvpartpair],entry.name);
		  
		  code = re_exec(patt);
		  if ( (code == 1) && xl[BACKVOL] >= 0 
		      &&   (entry.siteFlags[srvpartpair] & 
			    ((unsigned)VLSF_ZEROIXHERE >> xl[BACKVOL])))
		  {
		      if(entry.flags & VLF_BACKEXISTS) 
			volType = BACKVOL;
		      else
			code = 0;
		  }
		  else
		    code = 0;
		}		/*e*/
		
	      if (code != 1)
		{		/*a*/
		  sin = (struct sockaddr_in *) &entry.siteAddr[srvpartpair];
		  sprintf(patt, "0x%lx#%d#%s.readonly",
			  sin->sin_addr.s_addr,
			  entry.sitePartition[srvpartpair],entry.name);
		    
		  code = re_exec(patt);
		  if (code == 1) 
		    {
		      if(entry.flags & VLF_ROEXISTS) 
			volType = ROVOL;
		      else
			code = 0;
		    }
		}		/*a*/
		
	      switch(code)
		{
		case 0:
		  /* nothing matched */
		  break;
		    
		case 1:
		  /* Create a new volume dump entry for this freshly-matched
		   * volume and splice it on.
		   */

		  if ( matchedFlag != 0 )
		    {
		      dce_svc_printf(BAK_S_DUP_MATCH_FILESET_MSG, entry.name);
		      break;
		    }
		  matchedFlag = 1;
		    
		  total++;
		  tvd = (struct bc_volumeDump *) osi_Alloc(sizeof(struct bc_volumeDump));
		  if (tvd == (struct bc_volumeDump *)NULL)
		    return(BC_NOMEM);
		  bzero((char *)tvd, sizeof(struct bc_volumeDump));
		    
		  /* no check if the memory was obtained.
		   * no check of string lengths
		   * why not allocate the required string length?? PA
		   
		   * Should not use MAXNAMELEN, use BU_MAXNAMELEN if reqd.
		   */
		    
		  tvd->name = (char *) osi_Alloc(BU_MAXNAMELEN+1);
		  if (tvd->name == (char *)NULL)
		    return (BC_NOMEM);
		  strcpy(tvd->name, (char *)entry.name);
		  if(volType == BACKVOL)
		    strcat(tvd->name,".backup");
		    
		  if(volType == ROVOL)
		    strcat(tvd->name,".readonly");
		    
		  tvd->vid = entry.VolIDs[volType];
		  tvd->next = *avols;
		  tvd->entry = tve;
		  tvd->partition = entry.sitePartition[srvpartpair];
		  tvd->server = entry.siteAddr[srvpartpair];
		  *avols = tvd;
		  srvpartpair = entry.nServers;
		    
		  /*Advance to next VLDB entry*/
		  break;
		    
		default:
		  dce_svc_printf(BAK_S_INT_ERR_REGEXPR_MSG, mn, rn, code);
		  break;
		    
		}		/*end switch*/
	    }			/*for each srv/part pair*/
	    
	  /* Move on to the next volume spec associated with this volume
	   * set, if any.
	   */
	  tve = tve->next;
	}
	
      index = next_index;
    }				/*for each VLDB entry*/


  /* If we made it to here, we're in fat city.  Return the happy news to
   * our caller.
   */

  dce_svc_printf(BAK_S_TOTAL_NUM_FILESETS_MSG,total);
  return(0);
}				/*bc_EvalVolumeSet*/



#ifdef notdef
/*-----------------------------------------------------------------------------
 * bc_EvalVolumeSet
 *
 * Description:
 *	
 *
 * Arguments:
 *	aconfig	: Global configuration info.
 *	avs	: 
 *	avols	: Ptr to linked list of entries describing volumes to dump.
 *	uclient	: Ptr to Ubik client structure.
 *
 * Returns:
 *	0 on successful volume set evaluation,
 *	Lower-level codes otherwise.
 *
 * Environment:
 *	Expand only the single volume set provided (avs); don't go down its chain.
 *
 * Side Effects:
 *	None.
 *-----------------------------------------------------------------------------
 */

int bc_EvalVolumeSet(aconfig, avs, avols, uclient)
    struct bc_config *aconfig;
    register struct bc_volumeSet *avs;
    struct bc_volumeDump **avols;
    struct ubik_client *uclient;

{ /*bc_EvalVolumeSet*/

    static char	rn[] = "bc_EvalVolumeSet";  /*Routine name*/
    register long code;			    /*Result of various calls*/
    struct bc_volumeDump *tvd;		    /*Ptr to new dump instance*/
    struct sockaddr_in sa;
    struct bc_volumeEntry *tve;		    /*Ptr to new volume entry instance*/
    char patt[256];			    /*Composite regex; also, target string*/
    int volType;			    /*Type of volume that worked */
    char srvstring[64];			    /*Server portion of pattern*/
    char partstring[16];		    /*Partition portion of pattern*/
    static char	anything[] = ".*";	    /*Matches anything*/
    long index;				    /*Current VLDB entry index*/
    long count;				    /*Needed by VL_ListEntry()*/
    long next_index;			    /*Next index to list*/
    struct vldbentry entry;		    /*VLDB entry*/
    int	srvpartpair;			    /*Loop counter: server/partition pair*/
    long total = 0;
    error_status_t thisCommStatus;

    /*
     * Make sure we start off returning a null list of individual volumes for the expansion.
     */
    *avols = (struct bc_volumeDump *)0;

    /*
     * Sweep through the list of volume specs in the volume set, expanding each one in turn.
     */
    tve = avs->ventries;
    while (tve) {
	/*
	 * Expand current volume entry by generating the composite pattern and training
	 * the regex stuff to recognize it.
	 */
	/* compute server name based on wild-card or netaddress */
	sa = *((struct sockaddr_in *) &tve->server);
	if (strcmp(tve->serverName, anything)==0)
	    strcpy(srvstring, anything);
	else
	  sprintf(srvstring, "0x%lx", sa.sin_addr.s_addr);
	/* compute parition pattern based on wild-card or partition # */
	if (strcmp(tve->partname, anything) == 0)
	    strcpy(partstring, anything);
	else
	    sprintf(partstring, "%d", tve->partition);
	    
	sprintf(patt, "%s#%s#%s$",srvstring, partstring,tve->name);
	code = re_comp(patt);
	if (code) {
	    dce_svc_printf(BAK_S_CANT_COMPILE_REGEXPR_MSG, mn, rn, patt, code);
	    return(code);
	}

	/*
	 * Iterate through the VLDB, printing out each entry by index.  Variable next_index
	 * is set to the index of the next VLDB entry in the enumeration.
	 */
	for (index = 0; 1; index = next_index) {
	    bzero(&entry, sizeof(entry));
	    code = ubik_Call(VL_ListEntry,	/*Routine to invoke*/
			     uclient,		/*Ubik client structure*/
			     0,			/*Ubik flags*/
			     index,		/*Current index*/
			     &count,		/*Ptr to working variable*/
			     &next_index,	/*Ptr to next index value to list*/
			     &entry);		/*Ptr to entry to fill*/
	    if (code) {
		dce_svc_printf(BAK_S_VL_LISTENTRY_FAILED_MSG, mn, rn);
		PrintError("", code);
		return code;
	    }

	    /*
	     * If the next index is invalid, bail out now.
	     */
	    if (!next_index)
		break;

	    code = re_comp(patt);
	    if (code) 
	    {
		com_err(whoami, code,
			"Unable to compile regular expression '%s'", patt);
		return(code);
	    }
	    
	    /* We got an entry, now concatentate the pieces and try to match 
	     * it against the compiled pattern.
	     */
	    for (srvpartpair=0; srvpartpair < entry.nServers; srvpartpair++) {
		sprintf(patt, "0x%lx#%d#%s",entry.serverNumber[srvpartpair],
			entry.serverPartition[srvpartpair],entry.name);
		code = re_exec(patt);
		if ((code == 1) && (entry.serverFlags[srvpartpair] & ITSRWVOL)) volType = RWVOL;
		else {
		    sprintf(patt, "0x%lx#%d#%s.backup",
			    entry.serverNumber[srvpartpair],
			    entry.serverPartition[srvpartpair],entry.name);
		    code = re_exec(patt);
		    if ((code == 1) && (entry.serverFlags[srvpartpair] & ITSRWVOL)) {
			if(entry.flags & VLF_BACKEXISTS) volType = BACKVOL;
			else code = 0;
		    }
		    else
			code = 0;
		}
		if (code != 1) {
		    sprintf(patt, "0x%lx#%d#%s.readonly",
			    entry.serverNumber[srvpartpair],
			    entry.serverPartition[srvpartpair],entry.name);
		    code = re_exec(patt);
		    if (code == 1) {
			if(entry.flags & VLF_ROEXISTS) volType = ROVOL;
			else code = 0;
		    }
		}
		switch(code) {
		  case 0:
		    break;
		    
		  case 1:
		    /*
		     * Create a new volume dump entry for this freshly-matched volume
		     * and splice it on.
		     */
		    total++;
		    tvd = (struct bc_volumeDump *) osi_Alloc(sizeof(struct bc_volumeDump));
		    if (!tvd) {
			dce_svc_printf(BAK_S_CANT_ALLOC_FILESET_DUMP_MSG);
			return(-1);
		    }
		    bzero(tvd, sizeof(*tvd));

		    /* no check if the memory was obtained.
		     * no check of string lengths
		     * why not allocate the required string length?? PA
		     */

		    tvd->name = (char *) osi_Alloc(BU_MAXNAMELEN+1);
		    if (tcd->name == (char *)NULL)
		      return(BC_NOMEM);

		    voltype = RWVOL;

		    strcpy(tvd->name, entry.name);
		    if(volType == BACKVOL){
			strcat(tvd->name,".backup");
		    }
		    if(volType == ROVOL){
			strcat(tvd->name,".readonly");
		    }
		    tvd->vid = entry.volIDs[volType];
		    tvd->next = *avols;
		    tvd->entry = tve;
		    tvd->partition = entry.serverPartition[srvpartpair];
		    bzero((char *)&sa, sizeof(struct sockaddr_in));
		    sa.sin_addr.s_addr = entry.serverNumber[srvpartpair];
		    sa.sin_port = 0;	/* default FS port */
		    sa.sin_family = AF_INET;
		    bzero((char *)&sa, &(tvd->server), sizeof(afsNetAddr));
		    *avols = tvd;
		    
		    srvpartpair = entry.nServers; /*Advance to next VLDB entry*/
		    break;
		    
		  default:
		    com_err(whoami, code, "Internal error in regex package");
		    break;
		} /*end switch*/
	    } /*for each srv/part pair*/
	} /*for each VLDB entry*/

	/*
	 * Move on to the next volume spec associated with this volume set, if any.
	 */
	tve = tve->next;
    }

    /*
     * If we made it to here, we're in fat city.  Return the happy news to our caller.
     */
    dce_svc_printf(BAK_S_TOTAL_NUM_FILESETS_MSG,total);
    return(0);

}  /*bc_EvalVolumeSet*/

#endif /* notdef */
