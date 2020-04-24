/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: get_prirlm.c,v $
 * Revision 1.1.6.2  1996/10/03  15:06:34  arvind
 * 	Fix compiler warnings.
 * 	[1996/09/16  21:31 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:42  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/04/20  17:04 UTC  rps  /main/HPDCE02/2]
 *
 * 	merge fixes to mainline
 * 	HP 	[1995/04/20  17:02 UTC  rps  /main/MOTHRA_panic/4]
 *
 * 	free_aliases(): fix infinite loop
 * 	krb5_primary_realm(): copy length to returned value
 * 	HP 	[1995/04/03  23:59 UTC  sommerfeld  /main/MOTHRA_panic/3]
 *
 * 	add #ifdef OSF_DCE..
 * 	HP 	[1995/04/03  23:52 UTC  sommerfeld  /main/MOTHRA_panic/2]
 *
 * 	make thread safe, avoid duplicate caching, at the cost of some more
 * 	mallocs and frees and munges.  We may wish to back this change out &
 * 	add mutexes if it shows up on profiling..
 * 	HP 	[1995/03/10  20:41 UTC  sommerfeld  /main/MOTHRA_panic/1]
 * 	[1996/10/03  13:49:28  arvind]
 *
 * Revision 1.1.6.1  1996/05/10  13:19:25  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/12  22:11 UTC  pdd
 * 	fix functions that return no values
 * 	[1996/02/29  22:51 UTC  pdd  /main/HPDCE02/3]
 * 
 * 	Fix return non-value for read_aliases().
 * 	 	[1996/02/29  22:04 UTC  pdd  /main/HPDCE02/pdd_branch_CHFts17478/1]
 * 
 * 	    	HP revision /main/DCE_1.2/1  1996/01/03  21:16 UTC  psn
 * 	merge fixes to mainline
 * 	 	[1995/04/20  17:04 UTC  rps  /main/HPDCE02/2]
 * 
 * 	 	free_aliases(): fix infinite loop
 * 	 	krb5_primary_realm(): copy length to returned value
 * 	 	[1995/04/20  17:02 UTC  rps  /main/MOTHRA_panic/4]
 * 
 * 	 	add #ifdef OSF_DCE..
 * 	 	[1995/04/03  23:59 UTC  sommerfeld  /main/MOTHRA_panic/3]
 * 
 * 	 	make thread safe, avoid duplicate caching, at the cost of some more
 * 	 	mallocs and frees and munges.  We may wish to back this change out &
 * 	 	add mutexes if it shows up on profiling..
 * 	 	[1995/04/03  23:52 UTC  sommerfeld  /main/MOTHRA_panic/2]
 * 
 * 	 	New file: realm alias stuff.
 * 	 	Needs concurrancy protection (use pthread_once, actually..)
 * 	 	[1995/02/11  00:10 UTC  sommerfeld  /main/sommerfeld_krb5_alias_1/1]
 * 	[1996/05/09  21:49:39  arvind]
 * 
 * $EndLog$
 */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <ctype.h>
#ifdef OSF_DCE
#   include <dce/dce_cf.h>
#   include <dce/dce_cf_const.h>
#endif


#ifdef OSF_DCE
static void free_aliases(krb5_data *known_aliases)
{
    int i;

    for (i=0; known_aliases[i].data; i++)
    {
	free(known_aliases[i].data);
    }
    free(known_aliases);
}



static krb5_data *read_aliases(void)
{
    krb5_data *known_aliases = NULL;
    char **aliases;
    char **alias;
    char *cell_name;
    int i;
    
    
    error_status_t st, xst;
    int nalias = 0;
    
    dce_cf_get_cell_name(&cell_name, &st);

    if (st != 0)
	return NULL;
    
    dce_cf_get_cell_aliases(&aliases, &st);

    if ((st == 0) && (aliases != NULL)) 
    {
	for (alias = aliases, nalias=0;
	     *alias;
	      alias++, nalias++)
	    ;
	
	known_aliases = (krb5_data *)malloc (sizeof(krb5_data)*(nalias+2));
	if (known_aliases == NULL) 
	    goto errout;
	for (i=0; i<nalias+2; i++) 
	{
	    known_aliases[i].length = 0;
	    known_aliases[i].data = NULL;
	}
	st = dce_cell_to_realm(cell_name, &known_aliases[0].data);
	known_aliases[0].length=strlen(known_aliases[0].data);
	if (st != error_status_ok)
	    goto errout;
	
	for (i=0; i<nalias; i++) 
	{
	    st = dce_cell_to_realm(aliases[i], &known_aliases[i+1].data);
	    known_aliases[i+1].length=strlen(known_aliases[i+1].data);
	    
	    if (st != error_status_ok)
		goto errout;
	    
	}
	known_aliases[nalias+1].length = 0;
	known_aliases[nalias+1].data = NULL;
    }
    free(cell_name);
    return known_aliases;
  errout:
    free(cell_name);
    if (aliases)
	dce_cf_free_cell_aliases(aliases, &xst);
    if (known_aliases) 
    {
	free_aliases(known_aliases);
    }
    return NULL;
}
#endif

int krb5_primary_realm(krb5_data *r, krb5_data *rv)
{
#ifdef OSF_DCE
    int len = r->length;
    char *data = r->data;
    krb5_data *ret, *ali;
    char *ndata;
    int i;
    krb5_data *known_aliases;
    
    known_aliases = read_aliases();

    rv->data = NULL;
    rv->length = 0;
        
    if (!known_aliases)
	return 0;

    for (i=1; known_aliases[i].data; i++) 
    {
	krb5_data *ka = &known_aliases[i];
	
	if ((ka->length == len) &&
	    (memcmp(ka->data, data, len) == 0))
	{
	    len = known_aliases[0].length;
	    rv->data = malloc (len);
	    if (rv->data != NULL) 
	    {
		memcpy(rv->data, known_aliases[0].data, len);
		rv->length = len;
		free_aliases(known_aliases);
		return 1;
	    }
	}
    }
    free_aliases(known_aliases);    
#endif
    return 0;
}
