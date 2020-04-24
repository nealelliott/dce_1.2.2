/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21bind.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:15  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:53:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:45  root]
 * 
 * Revision 1.1.8.7  1994/09/06  15:24:56  keutel
 * 	OT 11933: local DSA is always read from the cache now
 * 	[1994/09/06  15:24:22  keutel]
 * 
 * Revision 1.1.8.6  1994/09/01  15:25:50  zeliff
 * 	Backing out accidental submission 11933
 * 	[1994/09/01  15:21:37  zeliff]
 * 
 * Revision 1.1.8.4  1994/06/21  14:46:37  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:38:40  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:47  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:46:37  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:31:20  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:25  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:28:03  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:32  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:31:01  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:06:49  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:52:30  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:13:51  marrek
 * 	insert <pthread.h>, <D00_dua_protect.h> and <D00_switch_protect.h>
 * 	use d21_l01_init()
 * 	[1993/02/02  08:11:45  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:46:44  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:24:33  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:09:31  marrek
 * 	November 1992 code drop
 * 	[1992/11/27  14:08:33  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:58:38  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:39:03  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:18:26  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:36:43  zeliff]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: d21bind.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:44 $";
#endif

/*----------------------------------------------------------------------*/
/*									*/
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG		*/
/* All Rights Reserved							*/
/*									*/
/* File Name	: d21bind.c						*/
/*									*/
/* Description  : Bind / Unbind functions in DUA			*/
/*									*/
/* Date         : December 12, 1987					*/
/*									*/
/* Author       : J. Heigert						*/
/*	(newer stuff: Jochen Keutel, SNI BA NM 123, Munich, Germany)	*/
/*									*/
/*----------------------------------------------------------------------*/

#include <gds.h>
#include <stdio.h>
#include <time.h>

#include <d21dua.h>

/*
**********************************************************************
*                                                                    *
*           D E F I N E S                                            *
*                                                                    *
**********************************************************************
*/
#define         D21_0_ILL_BIND_MODE             0
#define         D21_2_BIND_TO_DEFAULT_DSA       1
#define         D21_3_BIND_TO_NAMED_DSA         2
#define         D21_4_BIND_TO_ADDRESSED_DSA     3
#define         D21_5_BIND_TO_NAMED_DSA         4
#define         D21_CDS_BIND			5

/*
**********************************************************************
*                                                                    *
*       D E K L A R A T I O N of data  				     *
*                                                                    *
**********************************************************************
*/

#ifndef THREADSAFE
	static Bool first_bind = TRUE;
#endif

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_1_bind					*/
/*									*/
/* Description	: This function is used to bind to a DSA		*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : November 27, 1987					*/
/* Author       : J. Heigert						*/
/*	(newer stuff: Jochen Keutel, SNI BA NM 123, Munich, Germany)	*/
/*									*/
/*----------------------------------------------------------------------*/

d2_ret_val d21_1_bind (
  D21_p1 *parblk)
{
D21_p1          bindblk;         /* local copy of parblk                 */
signed16           save_version;
D2_a_value *       save_addr_dsa;
signed16           ret_value;
D2_a_value      *dsa_entry;
D2_name_string  dsa_name;
signed16           opmode = D21_0_ILL_BIND_MODE;
signed16           i;
const char function_name[] = "d21_1_bind";
D2_a_info * at_info;
char local_dsa[D2_DNL_MAX];
boolean local_dsa_found;

Bool            inv_cred;

D2_a_info *def_dsa_names = NULL;

/************************************************************************/

	       /* initialize logging and mutexes */
d21_L01_init();

DUA_TRACE_IAPL_ENTRY

/* log IAPL input */
d21_L02_trace_bind_input(parblk);

/*
check parameters as far as possible
*/
if ((parblk->d21_1dir_id < D2MINDIR_ID) ||
    (parblk->d21_1dir_id > D2MAXDIR_ID))
	{
	parblk->d21_1pbhead->d2_retcode = D2_CALL_ERR;
	parblk->d21_1pbhead->d2_errclass = D2_PAR_ERR;
	parblk->d21_1pbhead->d2_errvalue = D2_DIR_UNKNOWN;
	DUA_TRACE_WRONG_IAPL_INPUT(parblk->d21_1pbhead->d2_errvalue)
	return(D2_ERROR);
	}
if (!D2_CHECK_MINMAX(D21V022, D21V022, parblk->d21_1pbhead->d2_version))
	{			/* illegal version supplied		*/
	parblk->d21_1pbhead->d2_retcode = D2_CALL_ERR;
	parblk->d21_1pbhead->d2_errclass = D2_ILL_VERS;
	parblk->d21_1pbhead->d2_errvalue = D2_NOT_SPEC;
	DUA_TRACE_WRONG_IAPL_INPUT(parblk->d21_1pbhead->d2_errvalue)
	return(D2_ERROR);
	}

if (parblk->d21_1auth_mech != D2_ANONYMOUS && 
    parblk->d21_1auth_mech != D2_SIMPLE &&
    parblk->d21_1auth_mech != D2_DCE_AUTH &&
    parblk->d21_1auth_mech != D2_STRONG)
	{
	parblk->d21_1pbhead->d2_retcode = D2_CALL_ERR;
	parblk->d21_1pbhead->d2_errclass = D2_PAR_ERR;
	parblk->d21_1pbhead->d2_errvalue = D2_INVALID_AUTH_MECH;
	DUA_TRACE_WRONG_IAPL_INPUT(parblk->d21_1pbhead->d2_errvalue)
	return(D2_ERROR);
	}

/*
save supplied version and addr_dsa
*/
save_version = parblk->d21_1pbhead->d2_version;
save_addr_dsa = parblk->d21_1addr_dsa;
/*
initialize "error"-return
*/
parblk->d21_1bind_id = -1;

ret_value = D2_NOERROR;

/*
on first call to bind do some initializations
*/

#ifdef THREADSAFE
    {
        static pthread_once_t once_block = pthread_once_init;
        pthread_once(&once_block, d21_bh0_init_bind_handler);
    }
#else

if (first_bind)
	{
	d21_bh0_init_bind_handler() ;
	first_bind = FALSE ;
	}
#endif
       
/*
if not yet attached to ipc, then attach to ipc as client
*/
ret_value = d21_att_cl(parblk->d21_1pbhead);

if(ret_value != D2_NOERROR)
#ifdef OSFCDS
opmode = D21_CDS_BIND;
#else
return (D2_ERROR);
#endif

/*
prepare "local" parblk
*/
parblk->d21_1pbhead->d2_retcode =
parblk->d21_1pbhead->d2_errclass =
parblk->d21_1pbhead->d2_errvalue = D2_NO_ERR;
bindblk = *parblk;
/*
analyze service controls
*/
/*
set mode of operation
*/
if ( opmode == D21_0_ILL_BIND_MODE )
  do
	{

	if  ((bindblk.d21_1addr_dsa == NULL)         &&
	     (bindblk.d21_1name_dsa == NULL) )
		{		/* no DSA-address and no DSA-Name	*/
				/* bind to default dsa                  */
		opmode = D21_2_BIND_TO_DEFAULT_DSA;
		break;
		}

	if ( (bindblk.d21_1addr_dsa == NULL)         &&
	     (bindblk.d21_1name_dsa != NULL) )
		{               /* no DSA-address but DSA-Name          */
				/* bind to named dsa                    */
		opmode = D21_3_BIND_TO_NAMED_DSA;
		break;
		}

	if ( (bindblk.d21_1addr_dsa != NULL)         &&
	     (bindblk.d21_1name_dsa == NULL) )
		{               /* DSA-address but no DSA-Name          */
				/* bind to addressed "remote" dsa       */
		opmode = D21_4_BIND_TO_ADDRESSED_DSA;
		break;
		}

	if ((bindblk.d21_1addr_dsa != NULL)         &&
	     (bindblk.d21_1name_dsa != NULL) )
		{               /* DSA-address and DSA-Name             */
				/* bind to named dsa                    */
		opmode = D21_5_BIND_TO_NAMED_DSA;
		break;
		}
	}
  while (FALSE); /* end of do-while loop */

/*
set credentials if bind to dsa  requested
*/
if ( (parblk->d21_1u_credent == NULL) ||
     ( strcmp((char *)(parblk->d21_1u_credent->d2_c_name),D21_ANONYM) == 0 ) )
	{
	bindblk.d21_1u_credent = NULL ;
	}

switch (opmode)
{
case D21_CDS_BIND:
	DUA_TRACE_DUMMY_BIND_TO_CDS
	ret_value = d21_88_cds_bind(&bindblk);
	break;
	
case D21_2_BIND_TO_DEFAULT_DSA:
	{
	DUA_TRACE_BIND_TO_DEFAULT_DSA

/* will read local DSA from cache */

	ret_value =  d21_b_read_cache(parblk->d21_1pbhead,
		(D2_name_string)D2_N_LOC_DSA,parblk->d21_1dir_id, &at_info);

	if (ret_value != D2_NOERROR)
	  {
	  /* no local DSA is there */
	  local_dsa_found = FALSE;
	  }
	else
	  {
	  local_dsa_found = TRUE;
	  strcpy(local_dsa,
			(char *)at_info->d2_a_val->d2_at_value.d2_obj_name_a_v);
	  }

	if ( (parblk->d21_1dir_id > D2CACHE_MAXDIR_ID ) ||
	     ( (parblk->d21_1dir_id <= D2CACHE_MAXDIR_ID) &&
	       ( (ret_value = d21_b17_read_def_dsa(&def_dsa_names,&bindblk))
		  != D2_NOERROR
	   ) ) )
		{
		/* try to bind to a local dsa */

		if (local_dsa_found == FALSE)
		  {
		  /* no local DSA and no default DSA in the cache */
		  bindblk.d21_1pbhead->d2_retcode = D2_TEMP_ERR;
		  bindblk.d21_1pbhead->d2_errclass = D2_REM_ERR;
		  bindblk.d21_1pbhead->d2_errvalue = D2_NO_RESPONSE;
		  ret_value = D2_ERROR;
		  break;
		  }

		bindblk.d21_1name_dsa = (D2_name_string) local_dsa ;
		bindblk.d21_1addr_dsa = NULL ;
		/*
		try to bind
		*/
		ret_value = d21_bh1_exec_bind(&bindblk,FALSE) ;
		break;
		}

/*	default-dsas have been read from cache */
/*	step through all default-dsas and try to bind */

	inv_cred = FALSE;

	for (i = 0; i  < def_dsa_names->d2_a_no_val; i++)
		{
		dsa_entry = &def_dsa_names->d2_a_val[i];
		dsa_name = (D2_name_string) dsa_entry->d2_at_value.d2_oct_a_v;
		if (local_dsa_found == TRUE &&
		 d21_c82_dsname_cmp((D2_name_string)local_dsa,dsa_name) == TRUE)
			{
			/* local dsa */
			bindblk.d21_1name_dsa = dsa_name;
			bindblk.d21_1addr_dsa = NULL;
			ret_value = d21_bh1_exec_bind(&bindblk,FALSE);
			}
		else
			{
			/* non-local dsa */
			bindblk.d21_1name_dsa = dsa_name;
			/* read psap-address */
			ret_value = d21_b18_read_psap(bindblk.d21_1name_dsa,
			bindblk.d21_1dir_id, &(bindblk.d21_1addr_dsa),
			bindblk.d21_1pbhead);
			/*
			bindblk.d21_1addr_dsa now points to a psap_address
			*/
			if (ret_value == D2_NOERROR)
				{
				ret_value=d21_bh1_exec_bind(&bindblk,FALSE);
				}
			if (bindblk.d21_1addr_dsa != NULL)
			  {
			  free(bindblk.d21_1addr_dsa);
			  }
			}
			/*
			final error-codes already set,
			if read_psap not okay
			*/
		if (ret_value == D2_NOERROR)
			{
			break;
			}
		if (bindblk.d21_1pbhead->d2_errvalue ==
		      D2_CREDENTIALS_INVALID)
			{
			inv_cred = TRUE ;
			}
		}  /* end of for-loop */

	if (ret_value != D2_NOERROR)
		{
		/*
		try to bind to a local dsa
		(indicated by d21_1name_dsa = NULL and
			      d21_1addr_dsa = NULL)
		*/
		bindblk.d21_1name_dsa = NULL ;
		bindblk.d21_1addr_dsa = NULL ;
		/*
		try to bind
		*/
		ret_value = d21_bh1_exec_bind(&bindblk,FALSE) ;
		if (ret_value != D2_NOERROR &&
		   bindblk.d21_1pbhead->d2_errvalue == D2_CREDENTIALS_INVALID)
			{
			inv_cred = TRUE ;
			}
		}

	if (inv_cred == TRUE  &&  ret_value != D2_NOERROR)
		{
		/*
		if one of the default-dsas or local dsa returned
		"credentials_invalid" this will be the final result
		*/
		 bindblk.d21_1pbhead->d2_retcode = D2_CALL_ERR;
		 bindblk.d21_1pbhead->d2_errclass = D2_PAR_ERR;
		 bindblk.d21_1pbhead->d2_errvalue =
		       D2_CREDENTIALS_INVALID;
		}

	break;

	} /* end of D21_2_BIND_TO_DEFAULT_DSA */

case D21_3_BIND_TO_NAMED_DSA:
	{
	DUA_TRACE_BIND_TO_NAMED_DSA_1
	/*
	read "locality" of DSA  from DUA-cache
	*/
	if ( (parblk->d21_1dir_id > D2CACHE_MAXDIR_ID ) ||
	     (d21_b15_read_named_dsa(&bindblk) != D2_NOERROR) )
		{
		bindblk.d21_1pbhead->d2_retcode = D2_TEMP_ERR;
		bindblk.d21_1pbhead->d2_errclass = D2_REM_ERR;
		bindblk.d21_1pbhead->d2_errvalue = D2_NO_RESPONSE;
		ret_value = D2_ERROR;
		}
	else
		{
		/*
		d21_b15 has set d21_1addr_dsa
		(if local dsa : d21_1addr_dsa = NULL)
		*/
		ret_value = d21_bh1_exec_bind(&bindblk,FALSE);
		}
	if (bindblk.d21_1addr_dsa != NULL)
	  {
	  free(bindblk.d21_1addr_dsa);
	  }
	break;
	} /* end of D21_3_BIND_TO_NAMED_DSA */

case D21_4_BIND_TO_ADDRESSED_DSA:
	{
	DUA_TRACE_BIND_TO_ADDRESSED_DSA
	/*
	use supplied dsa-address for bind to remote dsa
	*/
	ret_value = d21_bh1_exec_bind(&bindblk,FALSE);
	break;
	} /* end of D21_4_BIND_TO_ADDRESSED_DSA */

case D21_5_BIND_TO_NAMED_DSA:
	{
	DUA_TRACE_BIND_TO_NAMED_DSA_2
	/*
	dsa-name and dsa-address supplied :
	try to read locality of dsa from duacache;
	if read fails : use supplied address
	*/
	if ( (parblk->d21_1dir_id > D2CACHE_MAXDIR_ID ) ||
	     (d21_b15_read_named_dsa(&bindblk) != 0)  )
		{       /* cant read dsa location       */
		/*
		set name of dsa to NULL
		*/
		bindblk.d21_1name_dsa = NULL;
		bindblk.d21_1addr_dsa = parblk->d21_1addr_dsa;
		}
	ret_value = d21_bh1_exec_bind(&bindblk,FALSE);
	if (bindblk.d21_1addr_dsa != NULL)
	  {
	  free(bindblk.d21_1addr_dsa);
	  }
	break;
	} /* end of D21_5_BIND_TO_NAMED_DSA */

default:
	{
	DUA_TRACE_UNKNOWN_OPMODE
	parblk->d21_1pbhead->d2_retcode = D2_CALL_ERR;
	parblk->d21_1pbhead->d2_errclass = D2_PAR_ERR;
	parblk->d21_1pbhead->d2_errvalue = D21_NO_SELECTION;
	ret_value = D2_ERROR;
	}

} /* end of switch (opmode) */

if (ret_value == D2_NOERROR)
	{               /* bind o.k.                    */
	parblk->d21_1pbhead->d2_retcode =
	parblk->d21_1pbhead->d2_errclass =
	parblk->d21_1pbhead->d2_errvalue = D2_NO_ERR;
	parblk->d21_1pbhead->d2_version = save_version;
	parblk->d21_1bind_id = bindblk.d21_1bind_id;
	parblk->d21_1ret_dsa = bindblk.d21_1ret_dsa;
	}
else    {               /* bind error                   */
	/*
	pass error-values in local parameterblock to caller
	*/
	parblk->d21_1pbhead = bindblk.d21_1pbhead;
	parblk->d21_1pbhead->d2_version = save_version;
	}

parblk->d21_1pbhead->d2_version = save_version;
parblk->d21_1addr_dsa= save_addr_dsa;

DUA_TRACE_IAPL_EXIT(parblk->d21_1pbhead->d2_errvalue)

return(ret_value);

} /* end of d21_1_bind */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_2_unbind					*/
/*									*/
/* Description	: This function is used to unbind from a DSA		*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : November 27, 1993					*/
/* Author       : J. Heigert						*/
/*	(newer stuff: Jochen Keutel, SNI BA NM 123, Munich, Germany)	*/
/*									*/
/*----------------------------------------------------------------------*/

d2_ret_val d21_2_unbind (
  D21_p2 *parblk)
{
const char function_name[] = "d21_2_unbind";
signed16           save_version ;
d2_ret_val ret_value;

DUA_TRACE_IAPL_ENTRY

/* log IAPL input */
d21_L03_trace_unbind_input(parblk);

save_version = parblk->d21_2pbhead->d2_version ;
parblk->d21_2pbhead->d2_retcode = D2_NO_ERR;
				/*					*/
				/* exec the unbind			*/
				/*					*/
d21_bh2_exec_unbind(parblk);

parblk->d21_2pbhead->d2_version = save_version;
if (parblk->d21_2pbhead->d2_retcode == D2_NO_ERR)
	{
	ret_value = D2_NOERROR;
	}
else
	{
	ret_value = D2_ERROR;
	}

DUA_TRACE_IAPL_EXIT(parblk->d21_2pbhead->d2_errvalue)

return(ret_value);
}	/* d21_2_unbind() */

