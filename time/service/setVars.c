/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: setVars.c,v $
 * Revision 1.1.4.2  1996/02/17  23:35:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:14:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:26:16  root]
 * 
 * Revision 1.1.2.4  1994/10/06  20:48:46  agd
 * 	expand copyright
 * 	[1994/10/06  14:33:36  agd]
 * 
 * Revision 1.1.2.3  1994/09/20  16:58:52  bowe
 * 	Needs to be able to get config info from dced. [CR 11240]
 * 	[1994/09/20  16:58:38  bowe]
 * 
 * Revision 1.1.2.2  1994/08/23  20:32:41  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:57:15  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:53:11  cbrooks]
 * 
 * Revision 1.1.2.1  1994/08/05  20:47:51  cbrooks
 * 	CR11240 - add dced config functionality
 * 	[1994/08/05  20:23:55  cbrooks]
 * 
 * $EndLog$
 */
/*  $Rev$
**
**  dts_dtsd_parse_entry 
**  
**  parse an attribute/pair, and return a data structure that contains
**  the parsed value, its DTS data type, and a pointer to the variable 
**  that should receive the new value.
*/

#include <dce/dce.h>
#include <dce/dce_utils.h>
#include <dce/assert.h>

#include <dts.h>

#include <dce/dtss_config.h>
#include <dce/dtssdef.h>
#include <dce/dtssmacros.h>
#include <dce/utc.h>

#include <string.h>
#include <stdlib.h>

#include <dtsd-hostdata.h>
#include <dcedtsmac.h>
#include <dcedtsmsg.h>
#include <dcedtssvc.h>
#include <stdio.h>

extern ShmBuf shmBuf ;

/*
 * prototypes 
 */

extern int ValidateCheckInterval( void * ) ;
extern int ValidateQueryAttempts( void * ) ;
extern int ValidateMinServers( void * ) ;
extern int ValidateCourierRole( void * ) ;
extern int ValidateTolerance( void * ) ;
extern int ValidateGlobalTimeout( void * ) ;
extern int ValidateLocalTimeout( void * ) ;
extern int ValidateMaxInaccuracy( void * ) ;
extern int ValidateSyncInterval( void * ) ;

int set_bin_reltime( const VarValues * );
int set_unsigned32( const VarValues * ) ;
int set_entryname( const VarValues *);
int set_enumeration( const VarValues * )  ;


static struct SetVars  setvar_table[] = {
    { "checkinterval", e_server_only, K_TYPE_BINRELTIME, (char *)0, 
	  K_CHECK_INTERVAL_ATTR, (void*)&shmBuf.mgtParams.checkInt,
  	ValidateCheckInterval
    },

    { "courierrole", e_server_only, K_TYPE_ENUMERATION, 
	  (char *)"Ecourier,noncourier,backup", 
	  K_COURIER_ROLE_ATTR,&shmBuf.mgtParams.courierRole, 
	  ValidateCourierRole
      },

    { "tolerance", e_client_and_server, K_TYPE_BINRELTIME, (char *)0, 
	  K_ERROR_TOLLERANCE_ATTR, (void*)&shmBuf.mgtParams.errorTolerance,
	  ValidateTolerance
  },

    { "globaltimeout",e_client_and_server, K_TYPE_BINRELTIME , 
         ( char *)0 , K_WAN_TIMEOUT_ATTR, (void *)&shmBuf.mgtParams.WANtimeOut,
     ValidateGlobalTimeout 
  },

    { "localtimeout", e_client_and_server, K_TYPE_BINRELTIME , 
	  (char *)0 , K_LAN_TIMEOUT_ATTR, (void*)&shmBuf.mgtParams.LANtimeOut,
    ValidateLocalTimeout
  },

    { "maxinaccuracy", e_client_and_server, K_TYPE_BINRELTIME, 
	  (char *)0, K_MAXIMUM_INACCURACY_ATTR, 
	  (void*)&shmBuf.mgtParams.maxInacc,
    ValidateMaxInaccuracy},

    { "queryattempts", e_client_and_server, K_TYPE_UNSIGNED32, (char *)0, 
	  K_QUERY_ATTEMPTS_ATTR, (void*)&shmBuf.mgtParams.repetitions,
     ValidateQueryAttempts},

    { "serverentry", e_server_only, K_TYPE_FULLNAME, (char *)0, 
	  K_SERVER_ENTRY_NAME, (void*)shmBuf.mgtParams.servEntryName,
     (ValidationFunction)0},

    { "serverprincipal", e_server_only, K_TYPE_FULLNAME, (char *)0, 
	  K_SERVER_PRINCIPAL_NAME, (void*)shmBuf.mgtParams.servPrincName,
    (ValidationFunction)0},

    { "servergroup", e_server_only, K_TYPE_FULLNAME, (char *)0, 
	  K_SERVER_GROUP_NAME, (void*)shmBuf.mgtParams.groupName,
     (ValidationFunction)0},

    { "minservers", e_client_and_server, 
	  K_TYPE_UNSIGNED32, (char *)0, 
	  K_SERVERS_REQUIRED_ATTR, (void*)&shmBuf.mgtParams.minServers,
    ValidateMinServers},

    { "syncinterval", e_client_and_server, 
	  K_TYPE_BINRELTIME, (char *)0, K_SYNCH_ATTR, 
	  (void*)&shmBuf.mgtParams.syncHold,
     ValidateSyncInterval}
};

static int ValidateString( const char *, char *);
static int ValidateUInt( long, char *) ;


#define N_SETTABLE_VARS	  (sizeof setvar_table / sizeof(struct SetVars))

#define strEQ(a,b) (strcmp((a),(b)) == 0)

/*
 * dts_dtsd_parse_entry( const char * name, char * value) 
 *
 * return pointer to structure describing the parsed variable name 
 * and its (binary) value
 */

void 
dts_dtsd_parse_entry( const char name[], char value[], VarValues * vp ) 
{
    int i ;
    static VarValues v ;
    int n ;
    boolean found_attr_entry = FALSE ;
    register ShmBuf * shmPtr = &shmBuf ;
    
    /* begin */


    dce_assert( dts_svc_handle, vp != 0);
    v.attribute_type = ~0 ;
    v.valid_value =  FALSE ;
    v.datap = 0 ;		/* point to nowhere */

    for (i = 0 ; i < N_SETTABLE_VARS; i++)
    {
	register struct SetVars *sp = &setvar_table[i];
	if (strEQ( sp->variable_name, name)) 
	{

	    /*
	     * we are only here if we are a clerk or a server.
	     * we can configure anything as a server. 
	     * Clerk values are more restrictive 
	     */

	    boolean32 clerk_ok = sp->valid_for_role == e_client_and_server || 
		sp->valid_for_role == e_client_only ;


	    if ( (IsClerk() && ! clerk_ok))
	    {
		dce_svc_printf( DTS_S_SERVER_ONLY_MSG, name) ;
		goto AssignReturn;
	    }
	    
	    found_attr_entry = TRUE ;
	    switch( (int)sp->vtype) 
	    {

	    case K_TYPE_BINRELTIME:
		v.attribute_type = K_TYPE_BINRELTIME ;
		if(! utc_mkascreltime(&v.u.reltime, (char *)value))
		    v.valid_value = TRUE ;
		break;

	    case K_TYPE_ENUMERATION:
		v.attribute_type = K_TYPE_ENUMERATION ;
		if (sp->valset != 0) 
		{
		    if ((n = ValidateString( value, sp->valset)) >= 0)
		    {
			v.u.enumvalue = n ;
			v.valid_value = TRUE ;
		    }
		    else
			v.u.enumvalue = UNKNOWN_ENUMERATION  ;
		}
		break;

	    case K_TYPE_UNSIGNED32:
		{
		    char  * lastp = 0 ;
		    v.attribute_type = K_TYPE_UNSIGNED32 ;
		    v.u.uintvalue = strtoul(value, &lastp, 10);
		    if ((v.u.uintvalue == 0 && lastp == value) ||
			*lastp != '\0')
		    {
			v.u.uintvalue = ~0UL ;
			v.valid_value = FALSE ;
		    }
		    else
		    {
			v.valid_value = TRUE ;
			if (sp->valset != 0)
			{
			    if (ValidateUInt(v.u.uintvalue, sp->valset))
				v.valid_value = TRUE;
			    else
				v.valid_value = FALSE ;
			}
		    }			    
		}
		break;

	    case K_TYPE_FULLNAME:
		{
		    if(strncmp(value, "/.:/", 4) == 0 ||
		       strncmp(value, "/.../", 5) == 0)
		    {
			v.u.stringvalue = dce_strdup(value) ;
			strcpy(v.u.stringvalue, value) ;
			v.valid_value = TRUE ;
		    }
		}
		
		break;

	    default:
		dce_svc_printf( DTS_S_INVALID_TYPE_MSG) ;
		goto AssignReturn ;
	    } /* switch */

	    /*
	     * store pointer to data element 
	     */

	    v.datap = sp -> data_element_p ;


	    /*
	     * do some more validation 
	     * if we still believe the entry to be valid
	     * invoke the validation function.
	     */


 	    if (v.valid_value) 
	    {
		if (sp -> F)	/* we have a validation function */
		{
		    if ( sp -> F ( (void *)(&v.u)))
			v.valid_value = TRUE ;
		}
	    }

	    break;		/* for */

	} /* if */
    } /* for */
    
    if (! found_attr_entry) 
    {
	dce_svc_printf( DTS_S_BAD_ATTR_MSG, name) ; 
    }
    else if (! v.valid_value)	/* found one, is it good */
    {
	dce_svc_printf( DTS_S_INVALID_ATTR_VALUE_MSG, value, name); 
    }

AssignReturn: 
	  ;
    *vp = v ;
    return ;
}

/*
 * ValidateString - validate string entry 
 */

static int ValidateString( const char * value, char * set)
{

    register char * p  = 0 ;
    char eval = *set++;
    int position = 0 ;
    char *tmp_set;
    
    /* begin */

    switch(eval) 
    {
    case 'E':			/* an enumerated value  */
    case 'e':
	
	tmp_set = dce_strdup(set);	/* strtok() cloobers its input */
	for (p = strtok(tmp_set,","); p ; p = strtok(NULL, ","), position++)
	{
	    if (strEQ(value, p))
	    {
		free(tmp_set);
		return position;
	    }
	}
	free(tmp_set);
	break;

    default:
	break;
    }
    
    return -1;
}

/*
 * ValidateUInt - validate an unsigned int
 * can have the following: 
 * B: high and low bounds
 * H: high bound
 * L: low bound
 */

static int ValidateUInt( long intvalue, char * set) 
{
    char eval = *set++;
    char ch = '\0' ;  
    unsigned long lowval, highval ;
    unsigned long * valp ;
    int rv = 0 ;
    
    /* begin */

    switch(eval) 
    {
    case 'B':
    case 'b':
	if(sscanf(set, "%lu , %lu", &lowval, &highval) != 2)
	    return 0 ;

	if (intvalue >= lowval && intvalue <= highval)
	    rv = 1;
	else 
	    rv = 0 ;
	break;
	
    case 'L':
    case 'l':
	ch = 'L';
    case 'H':
    case 'h':

	if (ch == '\0')
	{
	    ch = 'H';
	}

	valp = &highval ;
	if(sscanf(set, "%lu", valp) != 1)
	    return 0 ;
	    
	rv = ( ch == 'H') ? (intvalue <= *valp) : ( intvalue >= *valp) ;
	return rv;
	break;

    default:
	break;
    }

    return rv ;
}

/*
 * External Setting Routines
 */

int set_bin_reltime( register const VarValues * vp)
{
    /* convert the reltime field to a 32 bit hi/lo value */
    UTCValue * uvp = (UTCValue *)vp -> datap;
    extern int utc_comptime( UTCValue *, UTCValue *, long *, utc_t *) ;
    
    if (utc_comptime( uvp, (UTCValue *)0, (long *)0, (utc_t *)&vp->u.reltime))
	return 1 ;
    return 0;
}

int set_unsigned32( register const VarValues * vp)
{
    unsigned32 * u32p = (unsigned32 * )vp -> datap;
    *u32p = vp -> u.uintvalue ;
    return 0 ;
}

int set_entryname( register const VarValues * vp )
{
    strcpy( vp -> datap, vp -> u.stringvalue) ;
    free( vp -> u.stringvalue ) ;
    return 0 ;
}

int set_enumeration( register const VarValues * vp ) 
{
    int * ip = (int *)vp -> datap;
    *ip = (int)(vp -> u.enumvalue);
    return 0;
}


void dts_dtsd_set_entry( const VarValues * vp)
{
    
    switch((int)(vp -> attribute_type)) 
    {
    case K_TYPE_UNSIGNED32:
	set_unsigned32( vp ) ;
	break;
	
    case K_TYPE_BINRELTIME:
	set_bin_reltime( vp) ;
	break;

    case K_TYPE_FULLNAME:
	set_entryname( vp ) ;
	break;

    case K_TYPE_ENUMERATION:
	set_enumeration( vp ) ;
	break;

    default:
	dce_svc_printf(DTS_S_BAD_ATTR_TYPE_MSG);
	break;
    }
    return ;
}
