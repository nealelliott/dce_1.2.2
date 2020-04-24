/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21readc.c,v $
 * Revision 1.2.10.2  1996/02/18  18:17:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:10  marty]
 *
 * Revision 1.2.10.1  1995/12/08  15:54:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:23  root]
 * 
 * Revision 1.2.8.3  1994/05/10  15:54:02  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:32:33  marrek]
 * 
 * Revision 1.2.8.2  1994/03/23  15:14:33  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:28:50  keutel]
 * 
 * Revision 1.2.8.1  1994/02/22  18:42:10  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:34:49  marrek]
 * 
 * Revision 1.2.6.2  1993/08/11  06:30:35  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:58:15  marrek]
 * 
 * Revision 1.2.4.3  1992/12/31  19:48:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:25:47  bbelch]
 * 
 * Revision 1.2.4.2  1992/11/27  21:35:28  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:41:06  marrek]
 * 
 * Revision 1.2.2.3  1992/06/02  03:29:01  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:37:53  zeliff]
 * 
 * Revision 1.2.2.2  1992/04/27  19:57:07  melman
 * 	Files from Helmut's visit
 * 	[1992/04/27  19:52:16  melman]
 * 
 * Revision 1.2  1992/01/19  22:13:11  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21readc.c,v $ $Revision: 1.2.10.2 $ $Date: 1996/02/18 18:17:22 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*
* daton **************************************************************
*                                                                    *
* TYPE         :  MODUL                                              *
*                                                                    *
* NAME         :  d21readc.c                                         *
*                                                                    *
* AUTHOR       :  J.Heigert                                          *
* DATE         :  14.01.87                                           *
*                                                                    *
* COMPONENT    :  Directory Service-V2                               *
*                                                                    *
* DOK.-REF.    :  Design-Specification, Leistungsbeschreibung        *
*                                                                    *
* PRD#/VERS.   :                                                     *
*                                                                    *
* DESCRIPTION  :  cache read                                         *
*                                                                    *
* D2_SYSTEM-DEPENDENCES:                                             *
*                                                                    *
* HISTORY      :                                                     *
*                                                                    *
* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   *
*          |          |                             |    |           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include <d21dua.h>

/*
**********************************************************************
*                                                                    *
*       D E K L A R A T I O N of data                                *
*                                                                    *
**********************************************************************
*/

/********** LOCAL ***********/

static D2_a_type   a_type =  {sizeof(D2_SEE_ALSO) - 1,
					(Ob_id_string)D2_SEE_ALSO};
static D2_a_type   a_type1 =  {sizeof(D2_PSAP_ADDRESS) - 1,
					(Ob_id_string)D2_PSAP_ADDRESS};

/********** LOCAL **********/

static d2_ret_val d21_b_get_at_value( char * , D2_a_value ** , D2_pbhead * );

/***********************************************************************/

d2_ret_val d21_b_read_cache(
  D2_pbhead * pbhead,
  D2_name_string entry,
  signed16 dir_id,
  D2_a_info ** at_info)
{
D21_p3                  inread;
char                  *req_ptr, *res_ptr;
signed32                  res_len,req_len;
D2_pbhead               r_pbhead;
d2_ret_val                     ret_value;
D2_req_info r_info;
D2_c_res                cres;

 /* read from DUA-Cache  */

res_ptr = NULL;

r_info.d2_r_val_at = D2_R_VAL_TYP;
r_info.d2_r_no_at = 1;
r_info.d2_r_type_at = &a_type;

r_pbhead.d2_retcode  = D2_NO_ERR;
r_pbhead.d2_errclass = D2_NO_ERR;
r_pbhead.d2_errvalue = D2_NO_ERR;

/*
 Initialize inread parblk
*/
inread.d21_3bind_id = -1;
inread.d21_3c_a = NULL;
inread.d21_3pbhead = &r_pbhead;
inread.d21_3entry = entry;
inread.d21_3r_info = &r_info;
inread.d21_3res = NULL;
inread.d21_3err = NULL;
inread.d21_3cres = &cres;

/*
calculate length of request-apdu
*/

req_len = d21_ca_len_inread(&inread);
if ((req_ptr = D27_MALLOC(svc_c_sev_warning,req_len)) == NULL)
{   pbhead->d2_retcode = D2_TEMP_ERR;
    pbhead->d2_errclass = D2_SYS_ERR;
    pbhead->d2_errvalue = D21_ALLOC_ERROR;
    return(D2_ERROR);
		       /* no memory allocated by DUA, return immediately ! */
}

/*
build request-apdu, bt_ptr can be "NULL" (as in the case of CACHE-READ)
*/
d21_rd03_build_req(&inread,req_ptr,dir_id, -1);

 /*
   execute read
 */
 ret_value = d21_rd04_cache_read(&inread,req_ptr,req_len,&res_ptr,&res_len);

 if (ret_value != D2_NOERROR || r_pbhead.d2_retcode != D2_NO_ERR)
  { pbhead->d2_retcode = r_pbhead.d2_retcode;
    pbhead->d2_errclass = r_pbhead.d2_errclass;
    pbhead->d2_errvalue = r_pbhead.d2_errvalue;
    free(req_ptr);
    if (res_ptr != NULL) free(res_ptr);
    return(D2_ERROR);
   }

   /*
      move result to user-memory
   */
   ret_value = d21_rd05_move_ei_info_to_mem(&inread, res_ptr);
   if (ret_value != D2_NOERROR)
    { pbhead->d2_retcode = r_pbhead.d2_retcode;
      pbhead->d2_errclass = r_pbhead.d2_errclass;
      pbhead->d2_errvalue = r_pbhead.d2_errvalue;
      free(req_ptr);
      free(res_ptr);
      return(D2_ERROR);
     }
   free(req_ptr);
   if (memcmp(inread.d21_3res->d2_ei_ai->d2_a_type.d2_type,
	       D2_SEE_ALSO, sizeof(D2_SEE_ALSO)-1))
      {                       /* no or wrong attr. received */
	free(req_ptr);
	free(res_ptr);
	/*
	set : DSA_UNKNOWN
	*/
	pbhead->d2_retcode  = D2_CALL_ERR;
	pbhead->d2_errclass = D2_PAR_ERR;
	pbhead->d2_errvalue = D2_DSA_UNKNOWN;
	return(D2_ERROR);
	}
   /*
      copy attr. info
      assumption: attr-info size <= sizeof APDU-LEN
      res_len = inread.d21_3res->d2_ei_ai->d2_a_type.d2_typ_len + 
	     sizeof(D2_a_info) + sizeof(D2_a_value) + 
	     inread.d21_3res->d2_ei_ai->d2_a_val->d2_a_v_len;
   */
   memcpy(inread.d21_3res, inread.d21_3res->d2_ei_ai, sizeof(D2_a_info));
   *at_info = (D2_a_info *) inread.d21_3res;
   free(res_ptr);
   return(D2_NOERROR);
 }

d2_ret_val d21_rd04_cache_read(
  D21_p3          *parblk,
  char            *req_ptr,
  signed32          req_len,
  char            **res_ptr,
  signed32          *res_len )
{
d21_ipc_info     *ipc_info;
D21_req_info            req_info;
d2_ret_val                   res;

 /* read from DUA-Cache  */

/*
build association to CACHE
*/
if (d21_bh4_i_bind(D23_DCASID,&ipc_info,parblk->d21_3pbhead)!= D2_NOERROR)
  {
    /*
    final values in pbhead already set
    */
    return(D2_ERROR);
  }

/*
allocate 1 ipc block for result
*/
*res_len = ipc_info->d21_i_lenms;
if ((*res_ptr = D27_MALLOC(svc_c_sev_warning,*res_len)) == NULL)
  {
  *res_ptr = NULL;
  return (D21_ALLOC_ERROR);
  }

/*
set ipc-request block
*/
req_info.d21_r_opcode = D23_READ;
req_info.d21_r_bufapdu = req_ptr;
req_info.d21_r_lenapdu = req_len;
req_info.d21_r_ipcinfo = ipc_info;
/*
send request to cache
*/
if (d21_send_invoke(&req_info,ipc_info->d21_i_assid,
					parblk->d21_3pbhead) != D2_NOERROR)
	{
	/*
	final values in pbhead already set
	*/
	return(D2_ERROR);
	}
/*
get result from cache
*/

req_info.d21_r_bufapdu = *res_ptr;
req_info.d21_r_lenapdu = *res_len;
res = d21_recv_result(&req_info,ipc_info->d21_i_assid,parblk->d21_3pbhead);
/*
release association to cache immediatly
*/
d21_bh5_i_unbind(ipc_info->d21_i_assid);

*res_ptr = req_info.d21_r_bufapdu;
*res_len = req_info.d21_r_lenapdu;

if (res != D2_NOERROR)
	{
	return(D2_ERROR);
	}
 /* read from DUA-Cache  */
return(D2_NOERROR);

}

d2_ret_val d21_rd05_move_ei_info_to_mem(
  D21_p3 * parblk,
  char * result)
{
D2_pbhead       *pbhead;
signed32            mem_len;
char           *from = result;
char           * mem;
register D23_ds_name     *r_name;
D23_en_info     *en_info;
D2_ei_info      * mem1 ;
D2_a_info       *mem2;
unsigned16          tag;
D23_com_res * cm_res ;


  pbhead    = parblk->d21_3pbhead;
  mem_len   = d21_ca_len_ei_info(result);

  /* Allocate memory for results of IAPL* */
  if ((parblk->d21_3res = D27_MALLOC(svc_c_sev_warning,mem_len)) == NULL)
  {   pbhead->d2_retcode = D2_TEMP_ERR;
      pbhead->d2_errclass = D2_SYS_ERR;
      pbhead->d2_errvalue = D21_ALLOC_ERROR;
      return(D2_ERROR);
		    /* no memory allocated by DUA, return immediately ! */
   }


 mem = (char *) parblk->d21_3res;
 mem1 = parblk->d21_3res;
 en_info = (D23_en_info *) from;
 tag = en_info->d23_entag;
 mem    += sizeof (D2_ei_info);

  /*
  only EOM-tag is never expected 
  */
  /*
   analyze result
  */
  if (tag != D23_ENTTAG)
	{
	pbhead->d2_retcode  = D2_SYS_ERR;
	pbhead->d2_errclass = D2_INT_ERR;
	pbhead->d2_errvalue = D21_RESULTAPDU_INVALID;
	return(D2_ERROR);       /* there must be an ent-tag !!! */
	}

 for (D2_EVER)
 {
  mem1 ->d2_ei_next = (D2_ei_info *)NULL;
  mem1->d2_ei_ai = (D2_a_info *)NULL;
  mem1->d2_ei_fromentry = (Bool) (en_info->d23_enmas);
  from   += D23_S_EN;        /* point to next tag */
  r_name = (D23_ds_name *)from;
  tag    = r_name->d23_dstag;

  if (tag == D23_NAMTAG)   /* in entry info, name tag must come */
       {
		mem1->d2_ei_entry = (D2_name_string)mem;
		strcpy(mem, (char *)(r_name+1));
		mem += r_name->d23_dsoff;
		from += r_name->d23_dsoff + D23_S_NM;
	}
  else
	{
	pbhead->d2_retcode  = D2_SYS_ERR;
	pbhead->d2_errclass = D2_INT_ERR;
	pbhead->d2_errvalue = D21_RESULTAPDU_INVALID;
	return(D2_ERROR);       /* there must be an ent-tag !!! */
	} /* end of namtag */

  /* next free memory for attribute information */
  mem2 = (D2_a_info *)mem;
  tag = ((D23_at_type *)from)->d23_atttag;

  if (tag == D23_ATTTAG)
     {
	    /* move attribute-informations */
	 d21_ca2_attr_iget(&from, mem2,&mem);
	 mem1->d2_ei_ai = mem2 == (D2_a_info *)mem ? NULL : mem2;
     }
  else
      {
	if (tag == D23_CRSTAG)
		{
		/*
		no attribute-info returned
		*/
		   mem1->d2_ei_ai = (D2_a_info *) NULL;
		   break;   /* from for ever */
		}
	else
		{
		/*
		unexpected tag
		*/
		pbhead->d2_retcode  = D2_SYS_ERR;
		pbhead->d2_errclass = D2_INT_ERR;
		pbhead->d2_errvalue = D21_RESULTAPDU_INVALID;
		return(D2_ERROR);
		}
      }
  en_info = (D23_en_info *) from;
  tag = en_info->d23_entag;
  if (tag == D23_ENTTAG)
	{
         mem1 = mem1->d2_ei_next = (D2_ei_info *)mem;
	 mem += sizeof(D2_ei_info);
	}
  else
     { break; }
 }

/* copy common results */

  cm_res = (D23_com_res *) from;
  parblk->d21_3cres->d2_al_deref = cm_res->d23_ali_deref;

    /* performer is always NULL, as results are not signed */
  parblk->d21_3cres->d2_performer = NULL;

return(D2_NOERROR);
}  /* end of rd05_move_ei_info_to_mem  */

void d21_rd03_build_req(
  D21_p3 * parblk,
  char * req,
  signed16 dir_id,
  signed16 bind_id)
{
char           *next = req;
D23_inread      *mp_head;

/*
build first part of request
*/
mp_head = (D23_inread *)req;
mp_head->d23_Ehdr.d23_versno = D23_APDUV22;
mp_head->d23_Ehdr.d23_dirid  = dir_id;
mp_head->d23_Ehdr.d23_bindid = bind_id;

next += D23_S_INR;
d21_cm03_build_comarg( parblk->d21_3c_a, &next);

/*
move Distinguished name
*/
d21_cab_dsname_iput(parblk->d21_3entry,next,&next);
/*
move requested attribute types
     (with or without "pseudo-attribute")
*/
if ((parblk->d21_3c_a != (D2_c_arg *)NULL) && 
    (parblk->d21_3c_a->d2_serv_cntrls & D2_USEDSA) && 
    !(parblk->d21_3c_a->d2_serv_cntrls & D2_DONT_STORE) )
	{       /* with "pseudo-attribute */
	d21_rat_iput(parblk->d21_3r_info,next,&next,TRUE);
	}
else
	{
	d21_rat_iput(parblk->d21_3r_info,next,&next,FALSE);
	}
/*
and eom-tag
*/
d21_ca9_eom_iput(next,&next);

} /* end of build request */

void d21_rat_iput(
  D2_req_info * from,
  char * to,
  char ** next,
  Bool req_pseudo)
{
  D23_rq_info     * mp_rqinfo;
  D2_a_type         obj_id;
  register signed32      i,nattr;

   /* Memory check is to be incorporated for APDU Memory */

    mp_rqinfo = (D23_rq_info *)to;
    mp_rqinfo->d23_rqtag = D23_REQTAG;
    mp_rqinfo->d23_rqreq = from->d2_r_val_at;
    mp_rqinfo->d23_rqnbr =  nattr = from->d2_r_no_at;

    *next = (char *)(mp_rqinfo + 1);

    if (nattr > 0)
	{
	if (req_pseudo == TRUE)
		{
		  obj_id.d2_type = (Ob_id_string)D2_CACHE_ATTR;
		  obj_id.d2_typ_len = sizeof(D2_CACHE_ATTR)-1;
		  d21_cm_cpyobj(&obj_id, next);
		  mp_rqinfo->d23_rqnbr += 1;
		}
	for (i=0; i<nattr; i++)
	     {
		d21_cm_cpyobj(from->d2_r_type_at+i, next);
	      }
	}

} /* end of rat */

/*
* exon ***************************************************************
*                                                                    *
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_b18_read_psap()                                *
*                                                                    *
* AUTHOR       :  J.Forster,     Softlab GmbH.                       *
* DATE         :  09.08.88                                           *
*                                                                    *
* SYNTAX       :  Short d21_b18_read_psap(name_dsa, addr_dsa, pbhead)*
*        D2_name_string  name_dsa;                                   *
*        D2_a_value     *addr_dsa;                                   *
*        D2_pbhead      *pbhead;                                     *
*                                                                    *
* DESCRIPTION  :  Read Psap-Address from DUA-Cache                   *
*                                                                    *
* INPUT-PARAMETER :                                                  *
*                                                                    *
* OUTPUT-PARAMETER :                                                 *
*                                                                    *
* COMMENTS :                                                         *
*                                                                    *
* exoff **************************************************************
*/

d2_ret_val d21_b18_read_psap(
  D2_name_string name_dsa,
  signed16 dir_id,
  D2_a_value ** addr_dsa,
  D2_pbhead * pbhead)
{
D21_p3                  inread;
char                  *req_ptr, *res_ptr;
d2_ret_val ret_value;
signed32                  res_len,req_len;
D2_pbhead              r_pbhead;
D2_req_info r_info;

res_ptr = NULL;

/*
read from dua_cache; use "LOCAL-DSA" short form !
initialize inread parblk.
*/
r_info.d2_r_val_at = D2_R_VAL_TYP;
r_info.d2_r_no_at = 1;
r_info.d2_r_type_at = &a_type1;

r_pbhead.d2_retcode  = D2_NO_ERR;
r_pbhead.d2_errclass = D2_NO_ERR;
r_pbhead.d2_errvalue = D2_NO_ERR;

inread.d21_3bind_id = -1;
inread.d21_3c_a = (D2_c_arg *)NULL;
inread.d21_3pbhead = &r_pbhead;
inread.d21_3entry = name_dsa;
inread.d21_3r_info = &r_info;
inread.d21_3res = (D2_ei_info *)NULL;
inread.d21_3err = (D2_error *)NULL;

/*
calculate length of request-apdu
*/

req_len = d21_ca_len_inread(&inread);
if ((req_ptr = D27_MALLOC(svc_c_sev_warning,req_len)) == NULL)
{   pbhead->d2_retcode = D2_TEMP_ERR;
    pbhead->d2_errclass = D2_SYS_ERR;
    pbhead->d2_errvalue = D21_ALLOC_ERROR;
    return(D2_ERROR);
		       /* no memory allocated by DUA, return immediately ! */
}

/*
build request-apdu, 
*/
d21_rd03_build_req(&inread,req_ptr,dir_id, -1);

       /*
	execute read
	*/
	ret_value = d21_rd04_cache_read(&inread,req_ptr,req_len,
					&res_ptr,&res_len);

	if (ret_value == D2_NOERROR && r_pbhead.d2_retcode == D2_NO_ERR)
	{
	    if (d21_b_get_at_value(res_ptr, addr_dsa,
					   pbhead ) == D2_ERROR)
	      {  free(req_ptr);
		 free(res_ptr);
	       }
	}
	else
	{ 
          pbhead->d2_retcode = r_pbhead.d2_retcode;
          pbhead->d2_errclass = r_pbhead.d2_errclass;
          pbhead->d2_errvalue = r_pbhead.d2_errvalue;
	  free(req_ptr);
	  if (res_ptr != NULL) free(res_ptr);
	  return(D2_ERROR);
	}
       free(req_ptr);
       free(res_ptr);

 return(D2_NOERROR);
} /* d21_b18_read_psap */

static d2_ret_val d21_b_get_at_value(
  char * res_ptr,
  D2_a_value ** addr_dsa,
  D2_pbhead * pbhead)
{
 D23_at_type * at_type;
 D23_ds_name * ds_name;
 D23_av_value * av_value;
 signed32            len;

    ds_name = (D23_ds_name *) (res_ptr + D23_S_EN);
    res_ptr = (char *) ds_name + D23_S_NM + ds_name->d23_dsoff;
    at_type = (D23_at_type *) res_ptr;
    len = d21_ca4_attr_ilen_get((char **)&at_type);
    if ((*addr_dsa = D27_MALLOC(svc_c_sev_warning,len)) == NULL)
     { pbhead->d2_retcode = D2_TEMP_ERR;
       pbhead->d2_errclass = D2_SYS_ERR;
       pbhead->d2_errvalue = D21_ALLOC_ERROR;
       return(D2_ERROR);
      }
    at_type = (D23_at_type *) res_ptr;
    res_ptr += D23_S_ATT + at_type->d23_atoff + D23_S_AV;
    av_value = (D23_av_value *) res_ptr;
    (*addr_dsa)->d2_a_v_len = av_value->d23_avlen;
    (*addr_dsa)->d2_a_rep   = D2_PSAP_STX;
    (*addr_dsa)->d2_at_value.d2_psap_a_v = (Psap_string )(*addr_dsa + 1);
    memcpy((*addr_dsa)->d2_at_value.d2_psap_a_v,
	     av_value+1, av_value->d23_avoff);
    return(D2_NOERROR);
}


