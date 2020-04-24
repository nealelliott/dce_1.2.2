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
static char sccsid[] = "@(#)store.c	1.6 (93/01/25) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)store.c    1.6 1/25/93
NAME:		store.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions to handle forking for the group directive.

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
 *  store the details of a TC in a structure
 */
int store_tc(tc_store_ad, test_case, rdstore, num_sysid, line_num, element_num, num_testcases)
struct tc_store **tc_store_ad;
char *test_case;
struct sysid_store *rdstore;
int num_sysid;
int line_num;
int element_num;
int num_testcases;
{
    int count;

    DBUG(DBGFDESC) (stderr,"store_tc(tc_store_ad:%ld,%s,sysid_store:%ld,num_sys:%d,line:%d,element%d,num_tcs:%d)\n",(long)*tc_store_ad,test_case,(long)rdstore,num_sysid,line_num,element_num,num_testcases);

    /* reserve memory for testcase entry */
    if(*tc_store_ad == (struct tc_store *)0)
    {
	num_testcases = 0;
	*tc_store_ad = (struct tc_store *) TET_MALLOC(sizeof(struct tc_store));
    }
    else
    {
        *tc_store_ad = (struct tc_store *) TET_REALLOC((void *)(*tc_store_ad),( sizeof(struct tc_store)*(num_testcases+1)));
    }

#define tc	(*tc_store_ad)[num_testcases]

    tc.tc_name = (char *) TET_MALLOC(sizeof(char)*(strlen(test_case)+1));
    (void) strcpy(tc.tc_name, test_case); 

    tc.line_number = line_num;
    tc.element_number = element_num;

    tc.sysid_store = (struct sysid_store *) 0;
    tc.num_sysid = 0;

    if(num_sysid != 0)
    {
	for(count = 0; count < num_sysid; count++)
	    tc.num_sysid = store_sysid(&(tc.sysid_store), rdstore[count].sysid, tc.num_sysid);
    }

    /* Other elements of the tc_store structure are initialised */
    tc.process_ID = 0;
    tc.tmp_jnl_name = NULL;
    tc.duplicate = FALSE;
    tc.ret_val = SUCCESS;
    tc.override_eip = FALSE;

    if(*(tc.tc_name) == ':')
    {
    	    tc.ignore = TRUE;
    }
    else
    {
        if(check_line(tc.tc_name) == SUCCESS)
    	    tc.ignore = FALSE;
        else
    	    tc.ignore = TRUE;
    }

#undef tc

    num_testcases++;

    return(num_testcases);
}

/* Check that we haven't already stored a particular testcase. Checking the
 * element and line numbers is quicker than checking the testcase name.
 */
int isstored(tc_store_ad,buf,element,num_testcases)
struct tc_store **tc_store_ad;
int buf;
int element;
int num_testcases;
{
    int count;
    int ret_val = FALSE;


    for(count=0;count<num_testcases;count++)
    {
	if( (buf==(*tc_store_ad)[count].line_number)
	    &&
	    (element==(*tc_store_ad)[count].element_number) )
	{
	    ret_val = TRUE;
	    break;
	}
    }

    DBUG(DBGFDESC) (stderr,"isstored(tc_store_ad:%ld, %d, %d, %d) returns %s\n",(long)*tc_store_ad,buf,element,num_testcases, (ret_val == TRUE ? "TRUE" : "FALSE"));
 
    return(ret_val);
}


/* Clear memory used to store test case information.
 */
void clear_store(tc_store_ad, num_testcases)
struct tc_store **tc_store_ad;
int num_testcases;
{
    int count;
    if(*tc_store_ad == (struct tc_store *)0)
	return;

    DBUG(DBGFDESC) (stderr,"clear_store(tc_store_ad:%ld,%d)\n",(long) *tc_store_ad, num_testcases);

    for(count=0;count<num_testcases;count++)
    {

#define tc	(*tc_store_ad)[count]

	if(tc.tc_name != NULL)
	    TET_FREE((*tc_store_ad)[count].tc_name);
	if(tc.tmp_jnl_name != NULL)
	    TET_FREE(tc.tmp_jnl_name);
	if(tc.sysid_store != NULL)
	    clear_sysid_store(&(tc.sysid_store), tc.num_sysid);

#undef tc

    }
    TET_FREE(*tc_store_ad);
    *tc_store_ad = NULL;
}
