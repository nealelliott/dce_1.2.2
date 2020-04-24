/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: secutil.c,v $
 * Revision 1.1.4.2  1996/02/18  18:16:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:40  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:52:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:15  root]
 * 
 * Revision 1.1.2.8  1994/08/16  08:16:06  marrek
 * 	Fix for delta update (ot 11617).
 * 	[1994/08/12  12:00:08  marrek]
 * 
 * Revision 1.1.2.7  1994/08/11  07:58:10  marrek
 * 	Fix alignment problem (ot11618).
 * 	[1994/08/10  14:54:51  marrek]
 * 
 * Revision 1.1.2.6  1994/07/07  07:02:39  marrek
 * 	Add version string in July 1994 code drop.
 * 	[1994/07/07  06:58:46  marrek]
 * 
 * Revision 1.1.2.5  1994/06/21  14:46:31  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:55:22  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:45:03  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:49  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/11  12:30:49  keutel
 * 	fix for two failing subtree tests: OT 10602
 * 	[1994/05/11  12:24:09  keutel]
 * 
 * Revision 1.1.2.2  1994/05/10  15:53:32  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:30:06  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:14:11  keutel
 * 	creation
 * 	[1994/03/21  15:52:36  keutel]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char secutil_rcsid[] = "@(#)$RCSfile: secutil.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:16:06 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1993          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/************************************************************************/
/*								      	*/
/* Project      : GDS                                              	*/
/*								      	*/
/* File Name    : secutil.c                                           	*/
/*									*/
/* Description  : This file contains the functions needed by the	*/
/*		  security functions.					*/
/*								      	*/
/* Date         : 27.09.93						*/
/*								      	*/
/* Author       : Loose, SNI BU BA NM123                                */
/*								      	*/
/************************************************************************/

/************************************************************************/
/*								      	*/
/*			  I N C	L U D E	S			      	*/
/*								      	*/
/************************************************************************/

/*****  internal Includes    *****/

#include <d26dsa.h>
#include <d26schema.h>
#include <gds_sec.h>

/*****  Operating-System-Includes  *****/

#include <stdlib.h>
#include <string.h>

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

/******** EXPORT ********/
signed16 dsa_accept_simp_anon_sec_ctx(signed16 desired_sec_mech,
	D2_str *client_token, D2_str *return_token, D2_name_string *client_name,
	boolean *master_bind, D2_sec_status *sec_status);
signed16 dsa_conf_simp_anon_sec_ctx(signed16 desired_sec_mech,
	D2_str *sec_token, D2_sec_status *);
signed16 dsa_gen_simp_anon_sec_ctx(signed16 desired_sec_mech,
	D2_str *sec_token, D2_sec_status *sec_status);

/******** LOCAL  ********/
void d26_u101_search_params(D2_str *client_token, byte *s_in);

/************************************************************************/
/*								      	*/
/* Function Name : dsa_accept_simp_anon_ctx                            	*/
/*								      	*/
/* Description	 : This function is called by the security functions  	*/
/*		   in case of simple or anonymous credentials. It 	*/
/*		   checks the validity of the user password.	      	*/
/*								      	*/
/* Return Value  : D2_NOERROR in case of normal performance		*/
/*		   D2_ERROR if there where errors in internal search etc*/
/*								      	*/
/* Author        : Loose, SNI BU BA KP13                                */
/*								      	*/
/************************************************************************/
signed16 dsa_accept_simp_anon_sec_ctx(
	signed16     desired_sec_mech,	/* IN  - desired mechanism	*/
	D2_str 		*client_token, 	/* IN  - client token		*/
	D2_str 		*return_token,  /* OUT - security token -	*/
					/*       contains D23_simcr_info*/
	D2_name_string 	*client_name,	/* OUT - requestor name		*/
	boolean         *master_bind,	/* OUT - TRUE if master bind 	*/
					/*	 FALSE otherwise	*/
	D2_sec_status 	*sec_status)
{

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/
signed16         ret_value = D2_NOERROR;

				/* message blocks for DS_SEARCH   */
byte            *sm_in;
byte            *sm_out;
signed32         remlen;       	/* remaining message length       */

				/* parameters for DS_SEARCH       */
signed32         sm_out_len;   	/* total length of search result  */
				/* exact  dereferenced object     */
byte             exact_obj[D2_DNL_MAX + 1];
D26_attr_info    attr[D2_AT_MAX];
D26_acl          acl[D2_ACLIDX_MAX];
signed32         mk;
				/* object classes of binding      */
				/* object                         */
signed32         oclass[D26_OCL_VAL_COUNT + 1];
boolean          recurring;
String           record;
boolean          al_flag;
signed16         obj_idx;
byte 		*scan;
D23_en_info	*en_info;
D23_ds_name	*ds_name;
D2_name_string  own_name;
signed32	ret_size;
static char	*function = "dsa_accept_simp_anon_sec_ctx";

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

if (desired_sec_mech == D2_ANONYMOUS)
{
  return_token->d2_size = 0;
  return_token->d2_value = NULL;
  *master_bind = FALSE;
  strcpy((char *) *client_name,D26_ANONYMOUS);
  ret_value = D2_NOERROR;
}
else if (desired_sec_mech == D2_SIMPLE)
{

  /* check wether password is there */
  scan = (byte *) client_token->d2_value;
  scan += D23_S_SIMCR;
  scan += D23_S_NM + ((D23_ds_name *) scan)->d23_dsoff;
  if (((D23_pw_info *) scan)->d23_pwtag != D23_PWDTAG)
  {
    *master_bind = FALSE;
    strcpy((char *) *client_name,D26_ANONYMOUS);
  }
  else
  {  /* search for the entry and password */
			       /* allocate memory for search result   */
    if((sm_out = (byte *)D27_MALLOC(svc_c_sev_warning,D26_BLOCK_LEN)) == NULL)
    {
      sec_status->d2_retcode = D2_TEMP_ERR;
      sec_status->d2_errclass = D2_SYS_ERR;
      sec_status->d2_errvalue = D2_UNWILLING;
      ret_value = D2_ERROR;
    }
    else if((sm_in = (byte *)D27_MALLOC(svc_c_sev_warning,D26_BLOCK_LEN)) 
	    == NULL)
    {
      sec_status->d2_retcode = D2_TEMP_ERR;
      sec_status->d2_errclass = D2_SYS_ERR;
      sec_status->d2_errvalue = D2_UNWILLING;
      ret_value = D2_ERROR;
      free(sm_out);
    }
    else
    {
  
      sm_out_len = D26_BLOCK_LEN;
      d26_u101_search_params(client_token, sm_in);
			           /* call search                         */
      d26_base_object = (byte *)NULL;
      ret_value = d26_u00_search(FALSE,D23_BIND,sm_in,&sm_out_len,
	      &sm_out,&remlen,exact_obj,attr,oclass,acl,&mk,&recurring,
  	      &record,&al_flag,&obj_idx);
    
      if                  /*  system error in internal search     */
        (ret_value == D26_ERROR)
      {                   /*  error message "unavailable"         */
        sec_status->d2_retcode = D2_TEMP_ERR;
        sec_status->d2_errclass = D2_REM_ERR;
        sec_status->d2_errvalue = D2_NO_RESPONSE;
        free(sm_in);
        free(sm_out);
        return(D2_ERROR);
      } 
      else if (ret_value != D2_NO_ERR)    /*  other error occurred  */
      {                                   /*  invalid credentials   */
        sec_status->d2_retcode = D2_CALL_ERR;
        sec_status->d2_errclass = D2_PAR_ERR;
        sec_status->d2_errvalue = D2_CREDENTIALS_INVALID;
        *master_bind = FALSE;
        ret_value = D2_ERROR;
      } 
      else 
      {
        *master_bind = D2_ATTCMP(&d26_oct[oclass[0]].
		   d26c_ob_id,D2_ODSA) ? FALSE : TRUE;
  
        /* copy client name */
        scan = sm_out;
        en_info = (D23_en_info *) scan;
        ds_name = (D23_ds_name *) (en_info +1);
        strcpy((char *) *client_name, (char *) (ds_name + 1));
        free(sm_in);
        free(sm_out);
      }
    }
  }

  /* bind result contains DSA name 	     */
  /* -> write own DSA-name in return_token */

  own_name = (D2_name_string)(d26_myexact_name[0] == D2_EOS ? d26_myname :
			      d26_myexact_name);

  ret_size = D23_S_NM + D2_ALIGN(strlen((char *) own_name)+1);
  if (return_token->d2_size < ret_size &&
      (return_token->d2_value = (byte *) D27_REALLOC(svc_c_sev_warning, 
	return_token->d2_value, ret_size)) == NULL)
  {
    sec_status->d2_retcode = D2_TEMP_ERR;
    sec_status->d2_errclass = D2_SYS_ERR;
    sec_status->d2_errvalue = D2_UNWILLING;
    *master_bind = FALSE;
    ret_value = D2_ERROR;
  }
  else
  {
    return_token->d2_size = ret_size;
    scan = return_token->d2_value;

    ((D23_ds_name *) scan)->d23_dstag = D23_NAMTAG;
    ((D23_ds_name *) scan)->d23_dscomplete = TRUE;
    ((D23_ds_name *) scan)->d23_dslen = strlen((char *) own_name) + 1;
    ((D23_ds_name *) scan)->d23_dsoff = 
			D2_ALIGN(strlen((char *) own_name) + 1);
    scan += D23_S_NM;
    memcpy(scan,own_name,strlen((char *) own_name) + 1);
  }
}
else
{
  /* call error */
  sec_status->d2_retcode = D2_TEMP_ERR;
  sec_status->d2_errclass = D2_SYS_ERR;
  sec_status->d2_errvalue = D2_UNWILLING;
  ret_value = D2_ERROR;
}

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,sec_status->d2_errvalue));
return(ret_value);
}

/************************************************************************/
/*								      	*/
/* Function Name : d26_101_search_params                            	*/
/*								      	*/
/* Description	 : This function builds the input message for the	*/
/*		   internal search.					*/
/*								      	*/
/* Return Value  : - 							*/
/*								      	*/
/* Author        : Loose, SNI BU BA KP13                                */
/*								      	*/
/************************************************************************/

void d26_u101_search_params (
	D2_str	  *client_token,	/* IN  - client token		*/
					/*	 contains simple cred.  */
	byte   	  *s_in)		/* OUT - input message for search */

{                                   /*  d26_u101_search_params()       */

/**********************************************************************/
/*                  D E C L A R A T I O N S                           */
/**********************************************************************/

D23_ds_name	*in_name;      /* client name in input msg	 */
D23_pw_info	*pw_info;      /* password info			 */

D23_ds_name    *ds_name;       /* distinguished name of object   */
			       /* to be searched                 */
D23_rq_info    *rq_info;       /* pointer to requested info      */
D23_ft1_info   *ft1_info;      /* filter information block       */
D23_fi1_info   *fi1_info;      /* filter item block              */
D23_at_type    *at_type;       /* attr type block                */
D23_av_value   *av_value;      /* attribute value block          */
D23_com_arg    *com_arg;       /* common arguments               */
D23_srcharg    *srcharg;       /* search arguments               */

/**********************************************************************/
/*			      C	O D E				      */
/**********************************************************************/

/* scan input message */
in_name = (D23_ds_name *) (client_token->d2_value + D23_S_SIMCR);
pw_info = (D23_pw_info *) (client_token->d2_value + D23_S_SIMCR + D23_S_NM 
			   + in_name->d23_dsoff);

				    /*  fill invoke message for DS_SEARCH */
((D23_insearch *)s_in)->d23_Ihdr.d23_versno = D23_APDUMAX;

((D23_insearch *)s_in)->d23_Ihdr.d23_dirid  = 1; /* dummy dirid */

s_in += sizeof(D23_insearch);
				    /*  fill in common arguments   */

com_arg = (D23_com_arg *) s_in;
com_arg->d23_cmtag = D23_CMTAG;
com_arg->d23_cmali = D2_NOT_DEFINED;
com_arg->d23_cmctr.d23_scopt = 0L;
com_arg->d23_cmctr.d23_scpri = D2_PRIO_MEDIUM;           /* not used   */
com_arg->d23_cmctr.d23_sctlm = D2_T_UNLIMITED;
com_arg->d23_cmctr.d23_scslm = 1;
com_arg->d23_cmctr.d23_scref = 0;                    /* filler     */

com_arg->d23_cmopr.d23_opnm_rs = D2_NOT_STARTED;

s_in += sizeof(D23_com_arg);

				    /*  fill in search arguments      */
srcharg = (D23_srcharg *) s_in;

srcharg->d23_Itag = D23_ITAG;
srcharg->d23_Iali = TRUE;
srcharg->d23_Iobjsub = D2_BASE_OBJECT;


s_in += sizeof(D23_srcharg);
				    /*  put name to search message    */

ds_name = (D23_ds_name *) s_in;
ds_name->d23_dstag = D23_NAMTAG;
ds_name->d23_dscomplete = TRUE;
ds_name->d23_dslen = in_name->d23_dslen;
ds_name->d23_dsoff = in_name->d23_dsoff;
s_in += sizeof(D23_ds_name);

memcpy(s_in, in_name + 1, ds_name->d23_dslen);
				    /*  put Filter to message         */
s_in += ds_name->d23_dsoff;

				    /* put requested info (NONE) to message*/
rq_info = (D23_rq_info *) s_in ;
rq_info->d23_rqtag = D23_REQTAG;
rq_info->d23_rqreq = D2_R_TYP;
rq_info->d23_rqnbr = 0;

s_in += sizeof(D23_rq_info);

ft1_info = (D23_ft1_info *) s_in;
ft1_info->d23_fp1tag  = D23_PATTAG;
ft1_info->d23_fp1mod  = D2_ITEM;
ft1_info->d23_fp1count = 1;

s_in += sizeof(D23_ft1_info);

fi1_info = (D23_fi1_info *) s_in;

fi1_info->d23_fi1tag = D23_FIITAG;
fi1_info->d23_fi1match = D2_EQUAL;
fi1_info->d23_fi1syntx = D2_OCTET;

s_in += sizeof(D23_fi1_info);

at_type = (D23_at_type *)s_in;

at_type->d23_atttag = D23_ATTTAG;
at_type->d23_atlen = sizeof(D2_PWD)-1;
at_type->d23_atoff = D2_ALIGN(at_type->d23_atlen);

s_in += sizeof(D23_at_type);

memcpy(s_in, D2_PWD, at_type->d23_atlen);

s_in += at_type->d23_atoff;

av_value = (D23_av_value *) s_in;
av_value->d23_avlen = pw_info->d23_pwlen;
av_value->d23_avoff = pw_info->d23_pwoff;
av_value->d23_avrep = D2_OCTET;

s_in += sizeof(D23_av_value);

memcpy(s_in, pw_info + 1, pw_info->d23_pwlen);

s_in += av_value->d23_avoff;
((D23_eom *)s_in)->d23_eomtag = D23_EOMTAG;

}                                 /* d26_u101_search_params        */

/************************************************************************/
/*								      	*/
/* Function Name : dsa_conf_simp_anon_sec_ctx                          	*/
/*								      	*/
/* Description	 : This function is called by the security functions  	*/
/*		   in case of simple and anonymous credentials. 	*/
/*		   The function releases the memory allocated for the	*/
/*		   sec_token string.					*/
/*		   If DSA name is present in the bind result then 	*/
/*		   pointer to it is returned in return_sec_token other-	*/
/*		   wise this pointer is a NULL pointer.			*/
/*								      	*/
/* Return Value  : always D2_NOERROR					*/
/*								      	*/
/* Author        : Loose, SNI BU BA NM123                               */
/*								      	*/
/************************************************************************/
signed16 dsa_conf_simp_anon_sec_ctx(
	signed16 desired_sec_mech,	/* IN  - security mechanism	*/
	D2_str   *sec_token, 		/* IN  - bind result token	*/
	D2_sec_status *status)		/* OUT - error status		*/
{
/************************************************************************/
/*                       D E C L A R A T I O N S                       	*/
/************************************************************************/

static char	*function = "dsa_conf_simp_anon_sec_ctx";
signed16	ret_value = D2_NOERROR;

/************************************************************************/
/*                                C O D E                              	*/
/************************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
return(ret_value);

}

/************************************************************************/
/*								      	*/
/* Function Name : dsa_gen_simp_anon_sec_ctx                          	*/
/*								      	*/
/* Description	 : This function is called by the security functions  	*/
/*		   in case of simple and anonymous credentials. 	*/
/*		   The function returns D23_ds_name of D23_simcr_info	*/
/*		   structure used in the bind request.			*/
/*		   The memory for sec_token will be released in 	*/
/*		   dsa_conf_sec_ctx(). The length in sec_token is used 	*/
/*		   to calculate the length required for the bind	*/
/*		   request.						*/
/*								      	*/
/* Return Value  : D2_NOERROR						*/
/*		   D2_ERROR						*/
/*								      	*/
/* Date		 : 02.02.94						*/
/* Author        : Loose, SNI BU BA NM123                               */
/*								      	*/
/************************************************************************/
signed16 dsa_gen_simp_anon_sec_ctx(
	signed16 desired_sec_mech,	/* IN  - security mechanism	*/
	D2_str   *sec_token, 		/* OUT - bind request token	*/
	D2_sec_status *sec_status)	/* OUT - error status		*/
{
/************************************************************************/
/*                       D E C L A R A T I O N S                       	*/
/************************************************************************/

static char	*function = "dsa_gen_simp_anon_sec_ctx";
byte		*scan;
signed32	len;
signed16	ret_value = D2_NOERROR;

/************************************************************************/
/*                                C O D E                              	*/
/************************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

if (desired_sec_mech == D2_ANONYMOUS)
{
  sec_token->d2_size = 0;
  sec_token->d2_value = NULL;
}
else
{
  len = sizeof(D23_ds_name) + D2_ALIGN(strlen((char *)d26_myexact_name) + 1);
  if ((sec_token->d2_value = 
	(unsigned char *) D27_MALLOC(svc_c_sev_warning,len)) == NULL)
  {
      sec_status->d2_retcode = D2_TEMP_ERR;
      sec_status->d2_errclass = D2_SYS_ERR;
      sec_status->d2_errvalue = D2_UNWILLING;
      ret_value = D2_ERROR;
  }
  else
  {
    sec_token->d2_size = len;
    scan = sec_token->d2_value;
    ((D23_ds_name *) scan)->d23_dstag = D23_NAMTAG;
    ((D23_ds_name *) scan)->d23_dscomplete = TRUE;
    ((D23_ds_name *) scan)->d23_dslen = strlen((char *)d26_myexact_name) + 1;
    ((D23_ds_name *) scan)->d23_dsoff = 
				D2_ALIGN(((D23_ds_name *) scan)->d23_dslen);
    scan += D23_S_NM;
    strcpy((char *) scan,(char *)d26_myexact_name);
  }
}

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
return(ret_value);
}
