/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: x500convert.c,v $
 * Revision 1.1.8.2  1996/02/18  19:31:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:37  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:13:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:45  root]
 * 
 * Revision 1.1.6.6  1994/08/16  18:27:48  jd
 * 	drop from janet
 * 	[1994/08/13  16:05:32  jd]
 * 
 * Revision 1.1.6.5  1994/08/03  19:01:50  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:53:40  mccann]
 * 
 * Revision 1.1.6.4  1994/06/23  18:30:14  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:30:41  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:38:18  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:35  devsrc]
 * 
 * Revision 1.1.6.2  1994/05/12  21:11:23  peckham
 * 	Eliminate dnstables reference.
 * 	[1994/05/12  19:15:30  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:01:57  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:10:45  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:27:43  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:44:43  zeliff]
 * 
 * Revision 1.1.2.3  1992/03/22  21:58:51  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:24:49  weisman]
 * 
 * Revision 1.1.2.2  1992/01/22  23:01:35  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:17:11  melman]
 * 
 * $EndLog$
 */
/*
 * Module: x500convert.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 * x500convert.c
 *
 * This file contains conversion functions called by functions in x500parent.c:
 *
 * x500_rn_convert()
 * x500_rn_split_name()
 *
 * Note: this file was originally written by Siemens/Nixdorf and given
 * to DEC for integration in March 1991. It was rewritten by DEC in
 * October 1991 to account for a new gdad - gda_child interface. It
 * is maintained and delivered to OSF by DEC.
 */

#include <gda.h>
#include <gda_main.h>
#include <ctype.h>
#include <dce/rpc_cvt.h>
#include <errno.h>
#include <gda_xds.h>

/*
 *  Local function prototypes
 */
static void 
str2array (char  *,
           int   ,
           char  **);



/*
 * x500_rn_split_name - This function refers to in_out->unresolved_in_q. It
 *                      converts it into an unsigned char string which is
 *                      in printable format with = and / characters as
 *                      separators. It then splits this name into a global
 *                      (X.500) and DECdns part. It converts both these
 *                      names into structured format and puts the results
 *                      in in_out->resolved_out_q and in_out->unresolved_out_q.
 *                      The function puts the resolved name in the output
 *                      argument, iapl_name_cp. If CDS name conversion errors
 *			occur (not likely since they have made it through the
 *			API), return X500_ERROR; otherwise X500_NOERROR.
 *
 *			Assumptions:
 *			- name begins with /, leading /... has been removed
 *			- at least one name is typed
 *			- at least one CDS name doesn't contain '='
 * Modified: 28-may-93 (P.Keegan)
 * convert attributes to uncompressed oid format, 2.5.4.6, not 85.4.6
 */

int 
x500_rn_split_name (question_el_t  *in_out,
                    int            len,
                    char           *iapl_name_cp)
{
  char            *slash_p;
  char            *cp;
  dns_full_name_t  temp_opq_name;
  int              fname_len;
  char		 str_name[DNS_STR_FNAME_MAX];
  char		*str_name_p;
  int		 isEscaped;
  int		 isTyped;
  int              pure_global;
  char 		*lastSlash;

  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_gda,
		 svc_c_debug6,
		 " >x500_rn_split_name"));

  /*
   * Wire to null-terminated string
   */
  memcpy(str_name, in_out->unresolved_in_q.fn_name,
	 in_out->unresolved_in_q.fn_length);
  *(str_name + in_out->unresolved_in_q.fn_length) = '\0';

  str_name_p = str_name;
  if (*str_name_p == '/')
      str_name_p++;		/* advance past the / */

  isTyped = 0;		/* true if current component is typed */
  isEscaped = 0;		/* true if preceeding char was '\' */
  lastSlash = 0;		/* points to last '/' */
  slash_p = 0;		/* remember beginning of 1st untyped name */
  pure_global = TRUE;
  
  for (cp = str_name_p; *cp != '\0'; cp++) {
    switch (*cp) {
       case '/':
          if (!isEscaped) 
	      {
	    
		/*
		 * end of component - if 1st untyped, remember beginning 
		 */
		if (!isTyped) {
		  pure_global = FALSE;
		  if (slash_p == 0)
		      slash_p = lastSlash;
		}
		lastSlash = cp;
	      }
	  isEscaped = 0;
	  isTyped = 0;
	  break;
      case '=':
	  if (!isEscaped)
	      /*
	       * this component is typed 
	       */
	      isTyped = 1;
	  isEscaped = 0;
	  break;
      case '\\':
	  isEscaped = (isEscaped) ? 0 : 1;
	  break;
	}
  }

  if (!isTyped) 
      {
	/*
	 * treat end of string as a slash (ie a terminator) 
	 */
	pure_global = FALSE;
	if (slash_p == 0)
	    slash_p = lastSlash;
      }

  /*
   * Copy the resolved and unresolved names to the in_out area
   */
  if (pure_global == TRUE) 
      {
	in_out->unresolved_out_q.fn_length = 0;
      } 
  else 
      {
	
	/*
	 * Set unres_out ptr.           
	 */
	char *unres_out_cp;
	
	*slash_p     = '\0';
	unres_out_cp = (slash_p + 1);
	

	in_out->unresolved_out_q.fn_length = strlen(unres_out_cp);
	memcpy(in_out->unresolved_out_q.fn_name, unres_out_cp,
	       in_out->unresolved_out_q.fn_length);
      }

  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_gda,
		 svc_c_debug7,
		 "x500_rn_split_name: Res_out %s",
		 str_name));

  in_out->resolved_out_q.fn_length = strlen(str_name);
  memcpy(in_out->resolved_out_q.fn_name, str_name,
	 in_out->resolved_out_q.fn_length);
  COPY_ObjUID(cdsWildRoot, &in_out->resolved_out_q.fn_root);
  
  /*
   * String to internal -- this is where tags get replaced by oids
   */
  fname_len = sizeof(temp_opq_name);
  if (dns_CvtCDSFullToGDS((unsigned char *)str_name, 0, 1,
			  &temp_opq_name, &fname_len) != DNS_SUCCESS) 
      {
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_gda,
		       svc_c_debug2,
		       " <x500_rn_split_name: dns_CvtCDSFullToGDS failed - return(X500_ERROR)"));
	
	return(X500_ERROR);
      }

  /*
   * And back to string so we can convert it to IAPL* format
   */
  fname_len = sizeof(str_name);
  if (dnsCvtCDSFullToStr(&temp_opq_name, (unsigned char *)str_name,
			 &fname_len) != DNS_SUCCESS) 
      {
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_gda,
		       svc_c_debug2,
		       " <x500_rn_split_name: dnsCvtCDSFullToStr failed - return(X500_ERROR)"));
	
	return(X500_ERROR);
      }
  
  /*
   * Swallow the leading '/' and any escapes in the resolved X.500 name
   * (with object ids) while copying it to the caller's buffer
   */
  str_name_p = str_name;
  if (*str_name_p == '/')
      str_name_p++;		/* advance past the / */
  
  isEscaped = 0;
  for (cp = str_name_p; *cp; cp++) {
    if (*cp == '\\' && !isEscaped)
       	isEscaped = 1;
    else {
      *iapl_name_cp++ = *cp;
      isEscaped = 0;
    }
  }
  *iapl_name_cp = '\0';		/* nul terminator */
  
  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_gda,
		 svc_c_debug6,
    " <x500_rn_split_name"));

  return(X500_NOERROR);
}    

/*
 * x500_rn_convert() - This function takes raw information returned from the
 *                     X.500 directory system for the resolve name operation,
 *                     ie. D2_DNS_CELL and D2_DNS_REPLICA attribute values.
 *                     It transfroms this information and puts it into the
 *                     fields of the 'in_out' structure. When the function
 *                     returns, in_out->replicas_q points to an array whose
 *                     elements must be freed by calling rpc_ss_free() and
 *                     which must itself be freed by rpc_ss_free().
 *
 *                     Returns either DNS_NAMESERVERERRORS or DNS_SUCCESS.
 */


int 
x500_rn_convert (question_el_t    *in_out,
                 x500_dns_cell_t  *cell,
                 x500_replica_t   replicas[])
{
  int              ret_code;
  error_status_t    status;
  int		    i;
  char              **str_bindings;
  void		    *tower_p;

  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_gda,
		 svc_c_debug6,
		 " >x500_rn_convert"));
  
  /*
   * Convert namespace uuid from string and copy
   */
  uuid_from_string((unsigned char *)cell->nsp_uuid, &in_out->nsp_uuid_q, &status);
  if (status) 
      {	
	dce_svc_printf(CDS_S_GDA_BADNSUUID_MSG, (long)status);
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_gda,
		       svc_c_debug2,
		       " <x500_rn_convert: uuid_from_string (namespace) failed.\n\treturn(DNS_NAMESERVERERRORS)"));

	return(DNS_NAMESERVERERRORS);
      }
  /*
   * Convert root uuid & name if they were specified
   */
  if (cell->root_uuid != NULL) {
    uuid_from_string((unsigned char *)cell->root_uuid,
		     &in_out->dir_uuid_q, &status);
    if (status) 
	{   
	dce_svc_printf(CDS_S_GDA_BADNSUUID_MSG, (long)status);
	DCE_SVC_DEBUG((
	    cds__svc_handle,
            cds_svc_gda,
	    svc_c_debug2,
	    " <x500_rn_convert: uuid_from_string (root) failed.\n\treturn(DNS_NAMESERVERERRORS)"));

        return(DNS_NAMESERVERERRORS);
      }
    in_out->dir_name_q.fn_length = strlen(cell->root_name);
    memcpy(in_out->dir_name_q.fn_name, cell->root_name,
	   in_out->dir_name_q.fn_length);
  } else
      in_out->dir_uuid_q = in_out->nsp_uuid_q;
  
  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_gda,
		 svc_c_debug7,
		 "x500_rn_convert: Converted D2_DNS_CELL value to question_el_t format"));

  in_out->num_replicas_q = cell->num_replicas;
  in_out->replicas_q = (cds_ReplicaPointer_t **)malloc
      ((unsigned int) (cell->num_replicas * sizeof(cds_ReplicaPointer_t *)));
  if (in_out->replicas_q == NULL) 
      {
	dce_svc_printf(CDS_S_ALLOC_MSG);

	DCE_SVC_DEBUG((
	  cds__svc_handle,
	  cds_svc_gda,
	  svc_c_debug2,
	  " <x500_rn_convert: malloc failed - No space for replicas_q\n\treturn(DNS_NAMESERVERERRORS)"));

	return(DNS_NAMESERVERERRORS);
      }

  in_out->replicas_q[0] = NULL;
  ret_code = DNS_SUCCESS;

  for (i=0; i < cell->num_replicas; i++) {

    in_out->replicas_q[i] = (cds_ReplicaPointer_t *)rpc_ss_allocate
	((unsigned int)(sizeof(cds_ReplicaPointer_t)
			- sizeof(ndr_byte)
			+ X500_TOWER_LEN_MAX));
    if (in_out->replicas_q[i] == NULL) 
	{
	  dce_svc_printf(CDS_S_ALLOC_MSG);
	  ret_code = DNS_NAMESERVERERRORS;
	  break;
	}
    /* Ensure that next ptr in array is NULL, so */
    /* that you know how many to free later.     */
    if ((i + 1) < cell->num_replicas)
        in_out->replicas_q[i + 1] = NULL;

    in_out->replicas_q[i]->rp_type = replicas[i].type;
    if (replicas[i].type != RT_master && replicas[i].type != RT_readOnly) 
	{
	dce_svc_printf(CDS_S_GDA_BADREPLICATYPE_MSG);
        ret_code = DNS_NAMESERVERERRORS;
        break;
    }

    uuid_from_string((unsigned char *)(replicas[i].ch_uuid),
                     &(in_out->replicas_q[i]->rp_chid), &status);
    if (status) 
	{
	  dce_svc_printf(CDS_S_GDA_BADCHUUID_MSG);
	  ret_code = DNS_NAMESERVERERRORS;
	  break;
	}

    /*
     * If ch_name is a simple name, prepend its cellname before returning it
     */
    if (*replicas[i].ch_name != '/') {
      char *fn_name = (char *)in_out->replicas_q[i]->rp_chname.fn_name;

      memcpy(fn_name, "/...", 4);
      fn_name += 4;
      memcpy(fn_name, in_out->resolved_out_q.fn_name,
	     in_out->resolved_out_q.fn_length);
      fn_name += in_out->resolved_out_q.fn_length;
      *fn_name++ = '/';
      strcpy(fn_name, replicas[i].ch_name);
      in_out->replicas_q[i]->rp_chname.fn_length =
	  strlen((char *)in_out->replicas_q[i]->rp_chname.fn_name);
    } else {
      in_out->replicas_q[i]->rp_chname.fn_length =
	  strlen(replicas[i].ch_name);
      memcpy(in_out->replicas_q[i]->rp_chname.fn_name, replicas[i].ch_name,
	     in_out->replicas_q[i]->rp_chname.fn_length);
    }
    COPY_ObjUID(cdsWildRoot, &in_out->replicas_q[i]->rp_chname.fn_root);

    str2array(replicas[i].tower, replicas[i].tower_len,(char **)&str_bindings);
    if (dnsCvtStrToTower(str_bindings, (char **)&tower_p) != DNS_SUCCESS) 
	{
	  dce_svc_printf(CDS_S_GDA_BADTOWER_MSG);
	  ret_code = DNS_NAMESERVERERRORS;
	  break;
	}
    (void)RPTowerPack((Set_t *)tower_p, in_out->replicas_q[i]);

    if (str_bindings)
	free(str_bindings);
    if (tower_p)
	free(tower_p);
  }

  if (ret_code == DNS_NAMESERVERERRORS) {

    for (i=0; i < cell->num_replicas; i++) {
      
      if (in_out->replicas_q[i] == NULL)
	  break;
      
      free((char *)(in_out->replicas_q[i]));
    }
    free((char *)(in_out->replicas_q));
    
  };
  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_gda,
		 svc_c_debug6,
		 " <x500_rnconvert: return(ret_code = %d)",
		 ret_code));
  
  return(ret_code);
}				
/*
 * str2array() - Takes a tower string as stored in X.500 and returns
 *		 an array of pointers to the individual null-separated
 *		 tower strings.
 */

static void 
str2array (char  *str,
           int   len,
           char  **array)
{
  int ntowers, i;
  char *cur, *end;
  char prev;
  char **a;
  

  DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug8,
	    " >str2array"));

  ntowers = 0;
  prev = 0;
  for (i = 0; i < len; i++) {
    if (prev == 0 && str[i] != 0) {
      ntowers++;
      while(i < len && str[i] == 0)
	  i++;	/* advance past adj. nulls */
    }
    prev = str[i];
  }
  *array = calloc((size_t)ntowers + 1, sizeof(char *));
  a = (char **)*array;
  cur = str;
  end = cur + len;
  for (i = 0; i < ntowers; i++) {
    a[i] = cur;
    cur = strchr(cur, '\0');
    while (cur < end && *cur == 0)
	cur++;
  }
  
  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_gda,
		 svc_c_debug8,
		 " <str2array"));
}
