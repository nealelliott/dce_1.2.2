/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21sec.c,v $
 * Revision 1.1.4.2  1996/02/18  18:17:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:35  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:54:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:41  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:06  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:52  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:13  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:46:53  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:33:20  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:14:40  keutel
 * 	creation
 * 	[1994/03/21  15:52:50  keutel]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: d21sec.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:17:42 $";
#endif

/*----------------------------------------------------------------------*/
/*									*/
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG		*/
/* All Rights Reserved							*/
/*									*/
/* Project	: Security enhancements					*/
/*									*/
/* File Name	: d21sec.c						*/
/*									*/
/* Description  : The module contains functions for 			*/
/*		  security enhancements					*/
/*									*/
/* Date         : January 25, 1994					*/
/*									*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <d21dua.h>

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	dua_gen_simp_anon_sec_ctx			*/
/*									*/
/* Description	: This function generates D23_ds_name and D23_pw_info 	*/
/*		  part (D23_simcr_info) in case of SIMPLE auth,		*/
/*		  empty string in case of ANONYMOUS auth.		*/
/*									*/
/* Return Value	: D2_NOERROR						*/
/*		  D2_ERROR (malloc error)				*/
/*									*/
/* Date         : November 08, 1993					*/
/* Author       : Sanjay Jain, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

d2_ret_val dua_gen_simp_anon_sec_ctx(
  signed16 desired_auth_mech,
  D2_credent * my_creds,
  D2_str * sec_token)
{
signed32 cred_len;
char * c_ptr;

if (desired_auth_mech == D2_ANONYMOUS)
	{
	sec_token->d2_size = 0;
	sec_token->d2_value = NULL;
	}
else
	{ /* desired_auth_mech is D2_SIMPLE	*/
	cred_len = d21_ca_calc_credlen(my_creds);

	if ((c_ptr = D27_MALLOC(svc_c_sev_warning,cred_len)) == NULL)
		return(D2_ERROR);

	d21_ca8_cred_iput(my_creds, c_ptr);

	sec_token->d2_size = cred_len;
	sec_token->d2_value = (unsigned char *)c_ptr;
	}

return(D2_NOERROR);
}

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	dua_conf_simp_anon_sec_ctx			*/
/*									*/
/* Description	: This function confirms simple seurity context.	*/
/*		  It returns the DSA name of the bind result for	*/
/*		  simple credentials.					*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : January 25, 1994					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void dua_conf_simp_anon_sec_ctx(
  signed16 desired_auth_mech,	/* IN  */
  D2_str * sec_token,		/* IN  */
  D2_name_string * target_dsa)	/* OUT */
{
D23_ds_name * d23_ds_name;
char * dsa_name;

if (sec_token != NULL)
{
  if (desired_auth_mech != D2_ANONYMOUS)
  { /* desired_auth_mech is D2_SIMPLE	*/
    d23_ds_name = (D23_ds_name *)(sec_token->d2_value);
    if ((sec_token->d2_value != NULL) && (d23_ds_name->d23_dslen != 0))
    {
      dsa_name = (char *)(d23_ds_name + 1);
      strncpy((char *)(*target_dsa),dsa_name,d23_ds_name->d23_dslen);
					/* memory was allocated by bh1 */
    }
  } /* of simple credentials */
}

} /* end of dua_conf_simp_anon_sec_ctx */
