/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21mabind.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:47  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:20:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:43  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:55  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:28  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:39  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:36:54  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:51:28  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:39:33  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:37  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:07:05  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:00:18  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:13:14  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:00:37  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:43:47  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21mabind.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:05 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* NAME         : d21mabind.c                                         */
/*								      */
/* AUTHOR       : Volpers, D AP 11                                    */
/* DATE         : 08.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <dce/d21iapl.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <d20proto.h>
#include <gds_sec.h>

/* EXTERN DATA */

extern signed16 d20_auth_mech; /* authentication mechanism */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21d019_master_bind()                                 */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        08.01.88                                              */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will execute the "bind" operation to the*/
/*              target DSA (dsa_name or local_dsa). 		      */
/*		Target DSA could be dsa_name or local_dsa depending on*/
/*		if dsa_name is not NULL or it is NULL.		      */
/*              It reads the local dsa-name from the dsaname-file.    */
/*		It reads target DSA object from currently connected   */
/*		DSA (req. attr. is Password).			      */
/*              With these credentials it makes bind to the target dsa*/
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed16        bind_id;                              */
/*              Errcode         return_code;                          */
/*              char            *path;                                */
/*              D2_name_string  dsa_name;                             */
/*              signed16        dir_id;                               */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed16       *ma_bind_id;                           */
/*              char           *local_dsa;                            */
/*								      */
/* RETURNVALUE:							      */
/*              D2_NOERROR:     no errors                             */
/*              D2_ERROR:       bind failed                           */
/*								      */
/*exoff ***************************************************************/

d2_ret_val d21d019_master_bind(
  signed16 bind_id,
  Errcode * return_code,
  char * path,
  D2_name_string dsa_name,
  signed16 dir_id,
  signed16 * ma_bind_id,
  char * local_dsa)
{                               /*  d21d019_master_bind               */
static FILE        *fp;
char        filename[D27_LFILE_NAME];
				/* distinguished name of local DSA    */
D2_password pw;
D2_credent  user_cred;
D21_p1      parblk;
d2_ret_val ret_value;
D21_p3      pb ;
D2_ei_info  *results ;            /* req. attribute types */
D2_a_info   *attrinfo ;     /* attribute information queue */
D2_a_value  *aval ;         /* attribute informations */
D2_c_res	c_res;
Bool masterb_poss; 	/* flag to indicate if master bind is */
			/* possible to the target DSA or not  */

D2_sec_status status;
D2_name_string target_dsa;

static  D2_a_type   a_type = {sizeof(D2_PWD) - 1, (Ob_id_string)D2_PWD} ;
static  D2_req_info reqinfo ={ D2_R_VAL_TYP, 1, &a_type } ;
static  D2_pbhead   head = { D21V022 };
static  D2_c_arg    c_arg; 

			  /* read dsaname file */
return_code->retcode  = return_code->errclass = NOERR;
return_code->errvalue = NOERROR;
ret_value = D2_NOERROR;

sprintf(filename, "%s%s%s%d%s",path, D27_DSA_DIR, D27_SUBDSA_DIR, dir_id,
			       D27_DSANAME);

*ma_bind_id = -1;

if                              /*  open of dsaname file              */
  ((fp = fopen(filename, "r")) == (FILE *) NULL)
  {                               /*  set error values                */
    return_code->retcode  = PERMERR;
    return_code->errclass = SYSERR;
    return_code->errvalue = DS_DSA_NFOUND;
    return(D2_ERROR);
  }

/* read dsaname file */
if (fgets(local_dsa, D2_DNL_MAX - 1, fp) == NULL)
  {                               /*  set error values                  */
    return_code->retcode  = PERMERR;
    return_code->errclass = SYSERR;
    return_code->errvalue = DS_DSA_NFOUND;
    fclose(fp);
    return(D2_ERROR);
  }

fclose(fp);

target_dsa = (dsa_name != NULL) ? dsa_name : (D2_name_string)local_dsa;

/* if original bind has been done with SIMPLE or ANONYMOUS auth. mechs  */
/* try master bind with SIMPLE authentication mech.                     */
if (d20_auth_mech == D2_SIMPLE || d20_auth_mech == D2_ANONYMOUS)
  {
	 /* read password of target dsa                    */
	/* generate parameter block 			   */
  pb.d21_3pbhead = &head ;

  c_arg.d2_extension = NULL;
  c_arg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
  c_arg.d2_op_progress.d2_next_rdn = 0;
  c_arg.d2_al_rdn = 0;
  c_arg.d2_serv_cntrls = D2_NOCACHE | D2_NOREFERRAL | D2_USEDSA | 
		         D2_DONT_STORE | D2_PREF_ADM_FUN;
  c_arg.d2_priority = D2_PRIO_LOW ;
  c_arg.d2_time_limit = D2_T_UNLIMITED ;
  c_arg.d2_size_limit = D2_S_UNLIMITED ;
  c_arg.d2_scope_ref = D2_WORLD_SCOPE;

  pb.d21_3c_a = &c_arg ;
  pb.d21_3bind_id = bind_id ;
  pb.d21_3entry = target_dsa;
  pb.d21_3r_info = &reqinfo ;

  pb.d21_3err = NULL;
  pb.d21_3cres = &c_res;

  /* read password of the target DSA from currently connected DSA.	*/
  if ((ret_value = d21_3_read (&pb)) != D2_NOERROR)
	{                          /* set error codes                        */
    	if (pb.d21_3res != (D2_ei_info *) NULL)
    		free((char *) pb.d21_3res);

    	return_code->retcode  = PERMERR;
    	return_code->errclass = SYSERR;
    	return_code->errvalue = DS_DSA_NFOUND;
    	return(D2_ERROR);
	} 

  /* take result from read                 */
  results = pb.d21_3res ;
  attrinfo = results->d2_ei_ai;
  if (attrinfo == NULL)
    {  
    /* set error codes                        */
    free((char *) pb.d21_3res);

    return_code->retcode  = PERMERR;
    return_code->errclass = SYSERR;
    return_code->errvalue = DS_DSA_NFOUND;
    return(D2_ERROR);
    }

  if (D2_ATTCMP(&(attrinfo->d2_a_type), D2_PWD) != 0)
       			{
			/* set error codes                        */
    			free((char *) pb.d21_3res);

	  		return_code->retcode  = PERMERR;
	  		return_code->errclass = SYSERR;
	  		return_code->errvalue = DS_DSA_NFOUND;
	  		return(D2_ERROR);
			} 
  /* take result from read                 */
  aval = attrinfo->d2_a_val;
  pw.d2_p_len = aval->d2_a_v_len;
  pw.d2_p_pw  = aval->d2_at_value.d2_oct_a_v;

  parblk.d21_1u_credent = &user_cred;
  user_cred.d2_c_name   = target_dsa;
  user_cred.d2_c_passwd = &pw;

  parblk.d21_1auth_mech = D2_SIMPLE;
  }
else
  {
  if (d20_auth_mech == D2_DCE_AUTH)
    {
    /* check and set the master credentials for the target DSA */
    if (gds_set_master_bind(target_dsa, &masterb_poss, &status) == D2_ERROR)
      {
      /* set error values */
      return_code->retcode  = status.d2_retcode;
      return_code->errclass = status.d2_errclass;
      return_code->errvalue = status.d2_errvalue;
      return(D2_ERROR);
      }
    if (masterb_poss == FALSE)
      {
      /* master bind not possible with existing DCE credentials */
      /* set error values */
      return_code->retcode  = CALLERR;
      return_code->errclass = PARERR;
      return_code->errvalue = DS_MASTB_IMP;
      return(D2_ERROR);
      }

    /* proceed with DCE master bind */

    parblk.d21_1u_credent = NULL;
    parblk.d21_1auth_mech = D2_DCE_AUTH;
    }
  else
    {
    /* return ERROR that master bind not possible with the used */
    /* authentication mechanism.                                */
    return_code->retcode  = CALLERR;
    return_code->errclass = PARERR;
    return_code->errvalue = DS_MASTB_IMP;
    return(D2_ERROR);
    }
  } /* of if-else (credential kind) */

			  /* make bind as DSA to the given DSA        */

parblk.d21_1pbhead    = &head;
parblk.d21_1dir_id    = dir_id;

parblk.d21_1name_dsa  = target_dsa;
parblk.d21_1addr_dsa  = NULL;

if ((ret_value = d21_1_bind(&parblk)) != D2_NOERROR)
	{                          
	/* set error codes                        */
    	free((char *) pb.d21_3res);

    	return_code->retcode  = TEMPERR;
    	return_code->errclass = SYSERR;
    	return_code->errvalue = DS_NO_RESPONSE;
    	return(D2_ERROR);
	}
else
  	*ma_bind_id = parblk.d21_1bind_id;

				/*  return ret_value of "bind"        */
free((char *) pb.d21_3res);
return(ret_value);
}                               /*  d21d019_master_bind               */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21d022_unbind()                                      */
/*								      */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        16.03.89                                              */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will execute the "unbind" operation.    */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed16        bind_id;                              */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*              D2_NOERROR:     no errors                             */
/*              D2_ERROR:       unbind failed                         */
/*								      */
/*exoff ***************************************************************/

d2_ret_val d21d022_unbind(
  signed16 bind_id)
{                               /*  d21d022_unbind()                  */
static  D2_pbhead   head = { D21V022 };
D21_p2 parblk;
d2_ret_val ret_value = D2_NOERROR;

if (bind_id >= 0)
  {
  parblk.d21_2pbhead  = &head;
  parblk.d21_2bind_id = bind_id;
				/*  return ret_value of "unbind"      */
  ret_value = d21_2_unbind(&parblk);
  }
return(ret_value);
}                               /*  d21d022_unbind()                  */
