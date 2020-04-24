/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2ads_dn.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:08  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:51  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:44:34  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:28:09  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:15:44  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:36  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:17:18  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:38:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:46:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:07:31  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:38:45  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:13:30  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:11:11  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  16:02:34  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:54:12  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:09:26  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:03:51  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d2ads_dn.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:14 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYP		: C-FILE					      */
/*								      */
/* NAME         : d2ads_dn.c                                          */
/*								      */
/* AUTHOR       : Praveen Gupta (Siemens India)                       */
/* DATE         : 9. 1. 1990                                          */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.                   */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-01-09| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*       D E C L A R A T I O N of data				      */
/*                                                                    */
/**********************************************************************/

static char comment[100];

/******** LOCAL **********/

static signed32 d2a197_apdu_to_ids_dn (signed32,byte *,ids_dist_name *);
static signed32 d2a198_ids_dn_to_apdu (signed16,byte **,ids_dist_name *,byte **,
					signed32 *,byte *,signed32 *);


/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a_016_str_to_dn()                                     */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms a distinguished name from the string format used in*/
/*      the APDUs (typ!val!typ!.....!val where typ=1234.. and val is  */
/*      a printable string) to the ids_dist_name format.              */
/*                                                                    */
/*      ATT: The function allocates (lots of) memory which the caller */
/*           has to free after dn is no more used.                    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      str         = DN in string format                             */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      dn          = pointer to the ids_dist_name structure          */
/*      err_apdu    = error APDU (memory error or syntax error)       */
/*      err_len     = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: W. Schmid                                                 */
/**********************************************************************/

signed32 d2a_016_str_to_dn(
char		*str,
dua_dist_name   *dn,
byte     * err_apdu,
signed32       * err_len )
{
char                    *typptr[D2_NO_AVA], *valptr[D2_NO_AVA];
char			dn_str[2*D2_DNL_MAX];
register signed32            i, j , no_np, no_types, no_values;
signed32			 no_ava[2*D2_NP_MAX] ;
register char           *p_max;
register char           *p;
register dua_rdname     *rdn_ptr = NULL;
dua_assertion 		 *asr_ptr[2*D2_NP_MAX] ;
dua_assertion            *loop_asr_ptr;
Bool                    is_type;
char rep[D2_NO_AVA];
const char function_name[] = "d2a_016_str_to_dn";

strcpy(dn_str, str);

for (i=0;i<2*D2_NP_MAX;i++)
  {
  asr_ptr[i] = NULL;
  no_ava[i] = 0;
  }

no_values = 0;
no_types = 1;
typptr[0] = dn_str;
is_type = TRUE;

for (p = dn_str, p_max = p + strlen(dn_str), no_np = 0; p < p_max ; p++)
  { 
  /* if not TYPE_VAL_SEP (D2_T61_REP, D2_PRTBL_REP, D2_NUM_REP, D2_IA5_REP),
	AVA_SEP, RDN_SEP then continue */

  if ((*p == D2_T61_REP) || (*p == D2_PRTBL_REP) ||
      (*p == D2_NUM_REP) || (*p == D2_IA5_REP) )
    {
    if ( no_ava[no_np] >= D2_NO_AVA ) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

    rep[no_ava[no_np]] = *p;
    *p = D2_EOS;
    if ( is_type == TRUE )
      {
      /*   attr-value  is to follow 
           check length of naming attribute type */

      if (typptr[no_ava[no_np]] == (char *)NULL)
        {
	d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
			 D2_PAR_ERR, D21_BAD_NAME,err_len);
	D2_ASN1_ERR_RETURN_WITH_TRACE
        }
      valptr[no_ava[no_np]++] = p + 1 ;
      is_type = FALSE ;
      no_values++ ;
      }
    else 		/* no type before TYPE_VAL_SEP */
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    }

  if (*p == D2_AVA_SEP)
    {
    *p = D2_EOS;
    if (no_types != no_values)
      {
      d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
			 D2_PAR_ERR, D21_BAD_NAME, err_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }

    if (is_type == FALSE)   
      {
      /* attr.-type is to follow 
         check length of naming attribute value */

      if (valptr[no_ava[no_np]-1] == (char *)NULL)
        {
	d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
			 D2_PAR_ERR, D21_BAD_NAME, err_len);
	D2_ASN1_ERR_RETURN_WITH_TRACE
        }
        typptr[no_ava[no_np]] = p + 1 ;
        is_type = TRUE ;
        no_types++ ;
      }
    else 		/* no value before AVA_SEP */
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    }

  if (*p == D2_RDN_SEP)
    {
    *p = D2_EOS;
    if (no_types != no_values)
      {
      d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
			 D2_PAR_ERR, D21_BAD_NAME, err_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }

    if ( is_type == FALSE )
      {
      if (valptr[no_ava[no_np]-1] == (char *)NULL)
        {
	d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
			 D2_PAR_ERR, D21_BAD_NAME, err_len);
	D2_ASN1_ERR_RETURN_WITH_TRACE
        }
      }
    else 		/* no value before RDN_SEP */
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

    if ((asr_ptr[no_np] = D27_MALLOC(svc_c_sev_warning,no_ava[no_np]*sizeof(dua_assertion)))
								 == NULL)
      {
      d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			 D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }

    for (j=0, loop_asr_ptr = asr_ptr[no_np]; j<no_ava[no_np] ;
	 j++, loop_asr_ptr++)
      {
      if ((loop_asr_ptr->type.v = D27_MALLOC(svc_c_sev_warning,strlen(typptr[j]))) == NULL)
        {
        d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			 D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
        D2_ASN1_ERR_RETURN_WITH_TRACE
        }

      if (d27_302_str_oid(typptr[j], (D2_obj_id *)&(loop_asr_ptr->type))== -1)
        {
        d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
			      D2_PAR_ERR, D21_BAD_NAME,  err_len);
        D2_ASN1_ERR_RETURN_WITH_TRACE
        }

      switch (rep[j])
	{
	case D2_T61_REP:   loop_asr_ptr->value.mode = D2_T61;
			   break;
	case D2_PRTBL_REP: loop_asr_ptr->value.mode = D2_PRINTABLE;
			   break;
	case D2_NUM_REP:   loop_asr_ptr->value.mode = D2_NUMERIC;
			   break;
	case D2_IA5_REP:   loop_asr_ptr->value.mode = D2_IA5;
			   break;
	default:	   d27_010_set_error_apdu((D23_rserror *) err_apdu,
				D2_CALL_ERR,D2_PAR_ERR,D21_BAD_NAME,err_len);
			   D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if ((loop_asr_ptr->value.value.str_value =
				D27_MALLOC(svc_c_sev_warning,strlen(valptr[j])+1)) == NULL)
        {
        d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			      D2_SYS_ERR, D21_ALLOC_ERROR,
			       err_len);
        D2_ASN1_ERR_RETURN_WITH_TRACE
        }

      strcpy(loop_asr_ptr->value.value.str_value, valptr[j]) ;
      }
    is_type = TRUE;
    typptr[0] = p+1;
    no_types = 1;
    no_values = 0;
    no_np++;
    }
  } /* end of for-loop */

/* last RDN : */

if (no_types != no_values)
  {
  d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
	 D2_PAR_ERR, D21_BAD_NAME, err_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

if ( is_type == FALSE )
  {
  if (valptr[no_ava[no_np]-1] == (char *)NULL)
    {
    d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
			 D2_PAR_ERR, D21_BAD_NAME, err_len);
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  } 
else 	/* no value before RDN_SEP */
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

if ((asr_ptr[no_np] = D27_MALLOC(svc_c_sev_warning,no_ava[no_np]*sizeof(dua_assertion))) == NULL)
  {
  d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			 D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

for (j=0, loop_asr_ptr = asr_ptr[no_np]; j<no_ava[no_np] ;
     j++, loop_asr_ptr++)
  {
  if ((loop_asr_ptr->type.v = D27_MALLOC(svc_c_sev_warning,strlen(typptr[j]))) == NULL)
    {
    d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			 D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }

  if (d27_302_str_oid(typptr[j], (D2_obj_id *)&(loop_asr_ptr->type))== -1)
    {
    d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
			      D2_PAR_ERR, D21_BAD_NAME,  err_len);
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }

  switch (rep[j])
	{
	case D2_T61_REP:   loop_asr_ptr->value.mode = D2_T61;
			   break;
	case D2_PRTBL_REP: loop_asr_ptr->value.mode = D2_PRINTABLE;
			   break;
	case D2_NUM_REP:   loop_asr_ptr->value.mode = D2_NUMERIC;
			   break;
	case D2_IA5_REP:   loop_asr_ptr->value.mode = D2_IA5;
			   break;
	default:	   d27_010_set_error_apdu((D23_rserror *) err_apdu,
				D2_CALL_ERR,D2_PAR_ERR,D21_BAD_NAME,err_len);
			   D2_ASN1_ERR_RETURN_WITH_TRACE
	}

  if ((loop_asr_ptr->value.value.str_value =
			D27_MALLOC(svc_c_sev_warning,strlen(valptr[j])+1)) == NULL)
    {
    d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			      D2_SYS_ERR, D21_ALLOC_ERROR,
			       err_len);
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }

  strcpy(loop_asr_ptr->value.value.str_value, valptr[j]) ;
  }

no_np++;

if (no_np > 2*D2_NP_MAX)
  {
  d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
			 D2_PAR_ERR, D21_BAD_NAME, err_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

if ((rdn_ptr = D27_MALLOC(svc_c_sev_warning,no_np * sizeof(dua_rdname))) == NULL)
  {
  d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			 D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/*
   set up the dn structure
*/
dn->nb_rdnames = no_np ;
dn->rdnames = rdn_ptr ;

for(i = 0 ; i < no_np ; i++, rdn_ptr++ )
  {
  rdn_ptr->nb_assertions = no_ava[i] ;
  rdn_ptr->assertions = asr_ptr[i];
  sprintf (comment,"RDN %d value %s",i+1,asr_ptr[i]->value.value.str_value);
  }

return(D2_NOERROR) ;
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a017_dn_to_str()                                      */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms a distinguished name from the dua_dist_name format */
/*      used by the INRIA DUA to the string format used in the APDUs. */
/*      The content of dn is preserved (so that it can be freed later */
/*      on.                                                           */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      tag         = tag for apdu_structure to fill                  */
/*                    D23_NAMTAG: D23_ds_name structure               */
/*                    D23_ATTTAG: D23_at_info structure               */
/*                    D23_EOMTAG and default: D23_av_info structure   */
/*      dn          = pointer to the dua_dist_name structure          */
/*      act_apdu    = pointer to actual position in APDU where the    */
/*                    DN string can be stored                         */
/*      beg_apdu    = start address of APDU                           */
/*      max_apdu_len= max. length of allocated APDU memory            */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      act_apdu    = pointer to actual position in APDU where the    */
/*                    next component in the APDU can be stored        */
/*      beg_apdu    = (new) start address of APDU 		      */
/*      max_apdu_len= (new) max. length of allocated APDU memory      */
/*      err_apdu    = error APDU (memory error or syntax error)       */
/*      err_len     = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static char            dist_str[D2_DNL_MAX];


signed32 d2a017_dn_to_str(
  unsigned16          tag,
  dua_dist_name   *dn,
  Octet_string    *act_apdu,
  Octet_string    *beg_apdu,
  signed32             *max_apdu_len,
  Octet_string     err_apdu,
  signed32             *err_len )
{
Octet_string               next_pos;
signed32                        total_len;
D23_ds_name             *  name;
D23_av_value            *  avinfo;
register dua_rdname     *  rdn_ptr;
register dua_assertion  *  asr_ptr;
register char           *  s=dist_str;
char                       oid_str[D2_OBJ_IDL_MAX];
register signed32               i,j, name_len=0;
const char function_name[] = "d2a017_dn_to_str";

if (dn->nb_rdnames < 0)
{   return(D2_NOERROR);
}


/*
   convert to string
*/

switch (tag)
  {
  case D23_NAMTAG:
  case D23_RQTAG:
  case D23_BONTAG:
		total_len = sizeof(D23_ds_name);
		break;
  case D23_EOMTAG:
  default:
		total_len = sizeof(D23_av_value);
  } /* switch (tag) */
for (i = 0, rdn_ptr = dn->rdnames ; i < dn->nb_rdnames; i++, rdn_ptr++)
  for (j=0,asr_ptr=rdn_ptr->assertions;j<rdn_ptr->nb_assertions;j++,asr_ptr++)
    {
    name_len += ((asr_ptr->type.l)*3) +
		strlen(asr_ptr->value.value.str_value) + 2;
    }

name_len = D2_ALIGN(name_len);
if (name_len > D2_DNL_MAX)
  {
  d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
			  D2_PAR_ERR, D21_BAD_NAME,  err_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/*
   convert to string
*/

for (i = 0, rdn_ptr = dn->rdnames ; i < dn->nb_rdnames; i++, rdn_ptr++)
  {
  for (j=0,asr_ptr=rdn_ptr->assertions;j<rdn_ptr->nb_assertions;j++,asr_ptr++)
    {
    if (d27_301_oid_str((D2_obj_id *)&(asr_ptr->type), oid_str) == -1)
      {
      d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
	  	              D2_PAR_ERR, D21_BAD_NAME,  err_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    strcpy(s, oid_str);
    s += strlen(s);

    switch (asr_ptr->value.mode)
	{
	case D2_T61: 	   *s++ = D2_T61_REP;
			   break;
	case D2_PRINTABLE: *s++ = D2_PRTBL_REP;
			   break;
	case D2_NUMERIC:   *s++ = D2_NUM_REP;
			   break;
	case D2_IA5: 	   *s++ = D2_IA5_REP;
			   break;
	default:	   d27_010_set_error_apdu((D23_rserror *) err_apdu,
				D2_CALL_ERR,D2_PAR_ERR,D21_BAD_NAME,err_len);
	  		   D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    strcpy(s, asr_ptr->value.value.str_value) ;
    s += strlen(s);
    *s++ = D2_AVA_SEP;
    }
  *(s-1) = D2_RDN_SEP;
  }

if (i != 0)
  {
  (s)--;
  *s = D2_EOS;
  name_len = strlen(dist_str)+1;
  }

total_len += D2_ALIGN(name_len);

/*
   check whether DN can be store in APDU
  (beg_apdu = NULL means that memory has been checked before)
*/

if (beg_apdu != NULL)
  {
  if ( d2a033_check_memsize(total_len,beg_apdu,act_apdu,max_apdu_len,
			(D23_rserror *) err_apdu,err_len) != D2_NOERROR )
    {
     /* not enough memory  */
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  }

next_pos = *act_apdu + total_len;

switch (tag)
  {
  case D23_NAMTAG:
     name = (D23_ds_name *) *act_apdu;
     name->d23_dstag = D23_NAMTAG;
     name->d23_dscomplete = TRUE;
     name->d23_dsoff = D2_ALIGN(name_len);
     name->d23_dslen = name_len;
     *act_apdu = (Octet_string) ++name;
     break;
  case D23_RQTAG:
     name = (D23_ds_name *) *act_apdu;
     name->d23_dstag = D23_RQTAG;
     name->d23_dscomplete = TRUE;
     name->d23_dsoff = D2_ALIGN(name_len);
     name->d23_dslen = name_len;
     *act_apdu = (Octet_string) ++name;
     break;
  case D23_BONTAG:
     name = (D23_ds_name *) *act_apdu;
     name->d23_dstag = D23_BONTAG;
     name->d23_dscomplete = TRUE;
     name->d23_dsoff = D2_ALIGN(name_len);
     name->d23_dslen = name_len;
     *act_apdu = (Octet_string) ++name;
     break;
  case D23_EOMTAG:
  default:
     avinfo = (D23_av_value *) *act_apdu;
     avinfo->d23_avoff = D2_ALIGN(name_len);
     avinfo->d23_avlen = name_len;
     *act_apdu = (Octet_string) ++avinfo;

  } /* switch (tag) */

/*
  copy dist-name
*/

if (i != 0)
strcpy((char *)*act_apdu, dist_str);

/*
   set pointer to correct position in the APDU (for next tag)
*/

*act_apdu = next_pos;

return(D2_NOERROR) ;
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a121_dn_ids()                                         */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Converts dist-name from APDU format to IDS format.            */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*        scan :   pointer to octet_string of APDU;                   */
/*       ds_len:   count of bytes for Dist-name in APDU.              */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*       scan :   APDU-pointer pointing to octet after dist-name.     */
/*      dua_dn:   pointer to IDS-dist-name structure;                */
/*    err_apdu:   Err-APDU returned in the case of error;             */
/*     er_len :   contents indicate the length of error-APDU;         */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR      function successful                           */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a121_dn_ids(
  byte ** apdu,
  ids_dist_name * ids_dn,
  byte * err_apdu,
  signed32 * er_len )
{
dua_dist_name   d_name;
D23_ds_name   * ds_name = (D23_ds_name *) (*apdu);
D23_av_value * av_value;
byte * scan;
signed32 i;
const char function_name[] = "d2a121_dn_ids";

*apdu += D23_S_NM;
if ( ds_name->d23_dscomplete == TRUE )
  {
  if (ds_name->d23_dslen != 0)
    {
    if (d2a_016_str_to_dn((char *)*apdu, &d_name, err_apdu,
					er_len) == D2_ERROR)
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    if (dua_dist_name_ids(&d_name, ids_dn) != D2_NOERROR)
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

    dua_name_free(&d_name);

    *apdu += ds_name->d23_dsoff;
    }
  else
    {
    ids_dn->nb_rdnames = 0; /* dist. name is ROOT */
    if                            /*  no memory available               */
       ((ids_dn->rdnames = D27_CALLOC(svc_c_sev_warning,
					1, sizeof(ids_rdname))) == NULL)
      {                         /*  write error log                   */
      d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR,er_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    }
  }
else
  {
  if (d2a_016_str_to_dn((char *)*apdu, &d_name, err_apdu,
					er_len) == D2_ERROR)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  *apdu += ds_name->d23_dsoff;
  av_value = (D23_av_value *)*apdu;
  *apdu = (byte *) (av_value + 1);
  ids_dn->nb_rdnames = *( (signed16 *) *apdu);
  scan = (byte *) (*apdu + sizeof(signed16));
  *apdu += av_value->d23_avoff;
  if ( (ids_dn->rdnames = D27_MALLOC(svc_c_sev_warning,ids_dn->nb_rdnames * sizeof(ids_rdname)))
					== NULL)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  for (i=0; i<d_name.nb_rdnames; i++)
    {
    if (dua_rdname_ids (d_name.rdnames + i, ids_dn->rdnames + i)
							!= D2_NOERROR)
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    } /* end of for */

  if (d2a197_apdu_to_ids_dn(d_name.nb_rdnames,scan,ids_dn) != D2_NOERROR)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  }

return (D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a122_dn_apdu()                                        */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      converts dist-name from IDS-format to APDU-format.            */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      ids_dn : pointer to ids-dist-name structure.                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu   : pointer to APDU-address which is changed to point    */
/*               to the end of APDU-message.                          */
/*  beg_apdu   : pointer to APDU-address which points to beginning    */
/*               of APDU.					      */
/*  max_len    : pointer to interger indicating total size of APDU    */
/*  err_apdu   : pointer to Error-APDU returning Error in case of error*/
/*   apdu_len  : pointer to integer indicating length of Error-APDU   */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR      function successful                           */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a122_dn_apdu(
  unsigned16 tag,
  byte ** apdu,
  ids_dist_name * ids_dn,
  byte ** beg_apdu,
  signed32 * max_len,
  byte * err_apdu,
  signed32 * err_len )
{
signed32 i,offset;
dua_dist_name dua_dn;
D23_ds_name * name;
const char function_name[] = "d2a122_dn_apdu";

dua_dn.nb_rdnames = 0;
if ( ids_dn->nb_rdnames != 0 )
  {
  if ( (dua_dn.rdnames = D27_MALLOC(svc_c_sev_warning,ids_dn->nb_rdnames * sizeof (dua_rdname )))
					== NULL)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  for (i=0; i<ids_dn->nb_rdnames; i++)
    {
    if ( dua_rdname_dua (ids_dn->rdnames+i,dua_dn.rdnames+i)
							!= D2_NOERROR )
      {
      break;
      }
    } /* of for */

  dua_dn.nb_rdnames = i;
  } /* of ids->nb_rdnames != 0 */

offset = *apdu - *beg_apdu;

if (d2a017_dn_to_str(tag,&dua_dn,  apdu, beg_apdu, max_len,
		       err_apdu,err_len) == D2_ERROR)
	{ 
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}

switch (tag)
  {
  case D23_NAMTAG:
  case D23_RQTAG:
  case D23_BONTAG:
	name = (D23_ds_name *) (*beg_apdu + offset);
	name->d23_dscomplete = (dua_dn.nb_rdnames == ids_dn->nb_rdnames ) ?
				TRUE : FALSE;
	break;
  default:
	D2_ASN1_ERR_RETURN_WITH_TRACE

  } /* switch (tag) */

if ( name->d23_dscomplete == FALSE )
  {
  if (d2a198_ids_dn_to_apdu (dua_dn.nb_rdnames,apdu,ids_dn,beg_apdu,
				max_len,err_apdu,err_len) != D2_NOERROR)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  }

dua_name_free(&dua_dn);

return(D2_NOERROR);
}

static signed32 d2a197_apdu_to_ids_dn(
  signed32 num_dua_rdnames,
  byte * scan,
  ids_dist_name * ids_dn)
{
const char function_name[] = "d2a197_apdu_to_ids_dn";
signed32 i,j;
ids_rdname * rd_ptr;
ids_assertion * ass_ptr;

for (i=num_dua_rdnames,rd_ptr=ids_dn->rdnames+num_dua_rdnames;
			i<ids_dn->nb_rdnames;rd_ptr++,i++)
  {
  rd_ptr->nb_assertions = *((signed16 *)scan);
  scan += sizeof(signed16);
  if ((rd_ptr->assertions = D27_MALLOC(svc_c_sev_warning,
		rd_ptr->nb_assertions * sizeof(ids_assertion))) == NULL)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  for (j=0,ass_ptr=rd_ptr->assertions; j<rd_ptr->nb_assertions; j++,ass_ptr++)
    {
    ass_ptr->type.l = *((signed16 *)scan);
    scan += sizeof (signed16);
    if ( (ass_ptr->type.v = D27_MALLOC(svc_c_sev_warning,ass_ptr->type.l) ) == NULL)
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    memcpy((char *)ass_ptr->type.v,(char *)scan,ass_ptr->type.l);
    scan += D2_ALIGN(ass_ptr->type.l);
  
    ass_ptr->value.l = *((signed16 *)scan);
    scan += sizeof (signed16);
    if ( (ass_ptr->value.v = D27_MALLOC(svc_c_sev_warning,ass_ptr->value.l) ) == NULL )
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    memcpy((char *)ass_ptr->value.v,(char *)scan,ass_ptr->value.l);
    scan += D2_ALIGN(ass_ptr->value.l);
    }
  }
  
return(D2_NOERROR);
}



static signed32 d2a198_ids_dn_to_apdu(
  signed16 num,
  byte ** apdu,
  ids_dist_name * ids_dn,
  byte ** beg_apdu,
  signed32 * max_len,
  byte * err_apdu,
  signed32 * err_len )
{
const char function_name[] = "d2a198_ids_dn_to_apdu";
byte * scan;
D23_av_value * av_value;
signed32 len;
signed32 i,j;
ids_rdname * rd_ptr;
ids_assertion * ass_ptr;
signed16 * len_ptr;

len = sizeof(D23_av_value) + sizeof(signed16);

for (i=num,rd_ptr=ids_dn->rdnames+num;i<ids_dn->nb_rdnames;i++,rd_ptr++)
  {
  len+= sizeof(signed16);
  for (j=0,ass_ptr=rd_ptr->assertions;j<rd_ptr->nb_assertions;j++,ass_ptr++)
    {
    len+=  sizeof(signed16) + D2_ALIGN(ass_ptr->type.l)
         + sizeof(signed16) + D2_ALIGN(ass_ptr->value.l);
    }
  }

if ( d2a033_check_memsize(len,beg_apdu,apdu,max_len,
		(D23_rserror *) err_apdu,err_len) != D2_NOERROR )
  {
   /* not enough memory  */
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

av_value = (D23_av_value *) *apdu;
av_value->d23_avlen = len - sizeof(D23_av_value);
av_value->d23_avoff = D2_ALIGN(av_value->d23_avlen);
av_value->d23_avrep = D2_ASN1;
av_value->d23_avfil = 0;
*apdu += sizeof(D23_av_value);
scan = *apdu;

len_ptr = (signed16 *) scan;
*len_ptr = ids_dn->nb_rdnames;
scan += sizeof(signed16);

for (i=num,rd_ptr=ids_dn->rdnames+num;i<ids_dn->nb_rdnames;i++,rd_ptr++)
  {
  len_ptr = (signed16 *) scan;
  *len_ptr = rd_ptr->nb_assertions;
  scan += sizeof(signed16);
  for (j=0,ass_ptr=rd_ptr->assertions;j<rd_ptr->nb_assertions;j++,ass_ptr++)
    {
    len_ptr = (signed16 *) scan;
    *len_ptr = ass_ptr->type.l;
    scan += sizeof(signed16);
    memcpy((char *)scan,(char *)ass_ptr->type.v,ass_ptr->type.l);
    scan += D2_ALIGN(ass_ptr->type.l);

    len_ptr = (signed16 *) scan;
    *len_ptr = ass_ptr->value.l;
    scan += sizeof(signed16);
    memcpy((char *)scan,(char *)ass_ptr->value.v,ass_ptr->value.l);
    scan += D2_ALIGN(ass_ptr->value.l);
    }
  }

*apdu +=av_value->d23_avoff;
return(D2_NOERROR);
}
