/*
 * Copyright 1990 Open Software Foundation (OSF)
 * Copyright 1990 Unix International (UI)
 * Copyright 1990 X/Open Company Limited (X/Open)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of OSF, UI or X/Open not be used in 
 * advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission.  OSF, UI and X/Open make 
 * no representations about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 * OSF, UI and X/Open DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO 
 * EVENT SHALL OSF, UI or X/Open BE LIABLE FOR ANY SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef lint
static char sccsid[] = "@(#)remote.c	1.4 (93/01/24) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)remote.c    1.4 1/24/93
NAME:		remote.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions to store remote designation information.

MODIFICATIONS:
		DTET Development
		David G. Sawyer
		John-Paul Leyland
		UniSoft Ltd.

************************************************************************/

#include <tcc_env.h>
#include <tcc_mac.h>
#include <dtet/tet_jrnl.h>
#include <tcc_prot.h>


/*
 *  Check to see if the remote sysid 'sysid' is already in the list
 *  pointed to by 'sysid_store_ad'. If not add it and increases the total
 *  number of sysids in the list - 'num_des'.
 */
int store_sysid(sysid_store_ad, sysid, entry_num)
struct sysid_store **sysid_store_ad;
int sysid;
int entry_num;
{
    DBUG(DBGFDESC) (stderr,"store_sysid(sysid_store_ad:%ld,sysid:%d,entry_num:%d)\n",(long)*sysid_store_ad,sysid,entry_num);

    if(*sysid_store_ad == NULL)
    {
	entry_num = 0;
	*sysid_store_ad = (struct sysid_store *) TET_MALLOC(sizeof(struct sysid_store)*20);
    }
    else
    {
	/* check to see if it's already in the list */
        if(sysid_isstored(*sysid_store_ad,sysid,entry_num))
    	    return(entry_num);

	/* check to see if we should expand the meory allocated to the list */
	if(entry_num%20 == 0)
            *sysid_store_ad = (struct sysid_store *) TET_REALLOC(*sysid_store_ad, sizeof(struct sysid_store)*(entry_num+20));
    }


    (*sysid_store_ad)[entry_num].sysid = sysid;
    (*sysid_store_ad)[entry_num].logged_on = FALSE;
    (*sysid_store_ad)[entry_num].duplicate = FALSE;
    (*sysid_store_ad)[entry_num].override_eip = FALSE;

    return(++entry_num);
}


/*
 *  Take a list of comma separated remote ids and add them to the remote
 *  descriptor store specified by sysid_store.
 */
int str_to_sysids(sysid_store_ad,designations,num_des)
struct sysid_store **sysid_store_ad;
char *designations;
int num_des;
{
    char *ptr;
    char *temp_des;
    int count;


    DBUG(DBGFDESC) (stderr,"str_to_sysids(sysid_store_ad:%ld,\"%s\",%d)\n",(long)*sysid_store_ad,designations,num_des);


    temp_des = store_str(designations);
    ptr = strtok(temp_des,NAME_SEP_STR);

    do
    {
	/* check that the token consists purely of digits */
	for(count = 0; count < 3; count++)
	{
	    if(!isdigit(ptr[count]))
	    {
    		TET_FREE(temp_des);
		return(FAILURE);
	    }
	}

	/* Add remote number to the list - num_des = total num of entries */
	num_des = store_sysid(sysid_store_ad,atoi(ptr),num_des);
    }
    while((ptr = strtok((char *)NULL,NAME_SEP_STR)) != NULL);

    TET_FREE(temp_des);

    return(num_des);
}


/*
 *  Check that we haven't already stored a particular testcase. Checking the
 *  element and line numbers is quicker than checking the testcase name.
 */
int sysid_isstored(sysid_store,sysid, entry_num)
struct sysid_store *sysid_store;
int sysid;
int entry_num;
{
    int count;
    int ret_val = FALSE;

    for(count = 0; count < entry_num; count++)
    {
	if(sysid_store[count].sysid == sysid)
	{
	    ret_val = TRUE;
	    break;
	}
    }

    DBUG(DBGFDESC) (stderr,"sysid_isstored(sysid_store:%ld,sysid:%d,entry_num:%d) returns %s\n", (long)sysid_store, sysid, entry_num,(ret_val == TRUE ? "TRUE" : "FALSE"));

    return(ret_val);
}


/*
 *  Clear memory used to store remote designation information.
 *
 *  Note: no BAIL_OUTS are allowed in this function.
 */
void clear_sysid_store(sysid_store_ad,num_des)
struct sysid_store **sysid_store_ad;
int num_des;
{
    DBUG(DBGFDESC) (stderr,"clear_sysid_store(sysid_store_ad:%ld,%d)\n",(long) *sysid_store_ad,num_des);

    if(*sysid_store_ad != NULL)
    {
        TET_FREE(*sysid_store_ad);
        *sysid_store_ad = NULL;
    }
}
