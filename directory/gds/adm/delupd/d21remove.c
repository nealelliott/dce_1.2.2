/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21remove.c,v $
 * Revision 1.1.12.3  1996/02/18  19:42:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:50  marty]
 *
 * Revision 1.1.12.2  1995/12/08  15:20:25  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/04/26  19:41 UTC  jrr
 * 	Include errno.h for AIX.
 * 	[1995/12/08  14:52:44  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:11  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:14:26  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:57  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:36  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:42  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:37:14  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:53:47  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:39:58  marrek]
 * 
 * Revision 1.1.4.4  1993/01/29  08:38:48  marrek
 * 	removing of shadows of aliases handled.
 * 	[1993/01/28  10:43:07  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:43  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:07:13  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:00:53  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:13:38  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:00:54  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:45:12  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21remove.c,v $ $Revision: 1.1.12.3 $ $Date: 1996/02/18 19:42:07 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*								      */
/* NAME         : d21remove.o                                         */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 16.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <dce/d21iapl.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <d2dump.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define D21_SEARCH    "/REM"

#define D20_BUFSIZ      4096

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern char      *d20_beg1;             /* start address of DN-strings*/
extern char      *d20_beg2;             /* start address of DN-strings*/

extern char  	d20_m_dsa[];   	/* used by set_install_alias func.    */

/* LOCAL FUNCTIONS */

static d2_ret_val d21d021_rem_shadows( char * , Errcode * , D21_p8 * );
static d2_ret_val d21d023_handle_dns( D20_offset ** , signed32 , D21_p8 * );

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21_18_rmshd()                                         */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function looks for the appropriate shadwoing job in the  */
/*      shadowing job file and removes it.                            */
/*      If mode == SHD_AND_JOB the shadows will be looked for         */
/*      (DS_SEARCH). They will be removed in the DSA.                 */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*	signed16     bind_id;					      */
/*      signed16     version;                                         */
/*      Name_string  object;            object/subtree name to be     */
/*                                      removed                       */
/*      signed32     subset;            object interpretation         */
/*      Name_string  dsa_name;          master DSA name of shadow     */
/*                                      entries                       */
/*      signed32     mode;              JOB_ONLY or SHD_AND_JOB       */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode     *return_code;       error values                  */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_18_rmshd(
  signed16 bind_id,
  Errcode * return_code,
  signed16 version,
  Name_string object,
  signed32 subset,
  Name_string dsa_name,
  signed32 mode)
{
char     dir_path[D27_LFILE_NAME];
char    *path = dir_path;
Shd_job  job;
signed32 dir_id;
signed16 ma_bind_id = -1 ;
D21_p5   pb ;                 /* parameterblock for SEARCH     */
D21_p8   pbrem ;              /* parameterblock for REMOVE     */
char     filename[D27_LFILE_NAME];
char     local_dsa[D2_DNL_MAX + 1];
D2_Fi_item *p_item;
D2_a_value *p_val;
D2_c_res c_res;
char     norm_dsa_name[D2_DNL_MAX + 1];

static char function[] = "d21_18_rmshd";
static   D2_Fi_filter filter[3];/* filter information area */
static   D2_a_type    req_attr[2] = {
	{ sizeof(D2_ALIASED_OBJECT) - 1, (Ob_id_string) D2_ALIASED_OBJECT },
	{ sizeof(D2_MASTER_KNOWLEDGE) - 1, (Ob_id_string) D2_MASTER_KNOWLEDGE }
				    };
static   D2_req_info  reqinfo ={ D2_R_VAL_TYP, 2, req_attr } ;
static   D2_pbhead    head = { D21V022 };
static   D2_c_arg     c_arg = { NULL,{D2_NOT_STARTED,0},0,
	 D2_LOCALSCOPE | D2_NOREFERRAL | D2_USEDSA | D2_DONT_STORE | D2_DREFALIAS | D2_PREF_ADM_FUN, 
	 D2_PRIO_LOW, D2_T_UNLIMITED, D2_S_UNLIMITED, D2_WORLD_SCOPE};


DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

/* trace shadowing job                */
d21d010_trace_job(dsa_name, object, subset);

/* check version, bind_id-id; get path */
if (d21d006_param_check(bind_id, return_code, version, &path, &dir_id) == D2_ERROR)
	{   
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
   	return(D2_ERROR);
	}

/* check mode                         */
if (mode != JOB_ONLY && mode != SHD_AND_JOB)
	{   
	return_code->retcode  = CALLERR;
  	return_code->errclass = PARERR;
    	return_code->errvalue = DS_INVMODE;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
    	return(D2_ERROR);
	}

/* check object interpretation        */
if (subset != OBJECT && subset != OBJECT_SUBORDINATES)
	{   
	return_code->retcode  = CALLERR;
    	return_code->errclass = PARERR;
    	return_code->errvalue = DS_INV_SEARCH_SEL;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
    	return(D2_ERROR);
	}

/* define job to look for             */
if (object != NULL)
	{   
	strcpy(job.shd_objname,(char *)object);
   	d20_norm(job.shd_objname, strlen(job.shd_objname));
	}
else
	*(job.shd_objname) = D2_EOS;

job.shd_subset = subset;
strcpy(job.shd_dsa,(char *)dsa_name);
d20_norm(job.shd_dsa, strlen(job.shd_dsa));

/* remove shadowing job from shadowing*/
/* job file                           */
if (d21d005_upd_job_file(bind_id, REMOVE, &job, path, dir_id, return_code)
			 == D2_ERROR)
	{   
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
   	return(D2_ERROR);
	}

if (mode == SHD_AND_JOB)
	{                               
	/*remove shadows in DSA               */
	/*make bind as master dsa             */
    	if ((d21d019_master_bind(bind_id, return_code, path, dsa_name, dir_id,
			     &ma_bind_id, local_dsa)) == D2_NOERROR)
    		{   
		d20_norm(local_dsa, strlen(local_dsa));

		strcpy(norm_dsa_name,(char *)dsa_name);
		d20_norm(norm_dsa_name, strlen(norm_dsa_name));

		if (strcmp(norm_dsa_name, local_dsa) == 0)
			{                       
			/* --> objects from local (==MASTER) DSA */
			/*     should be removed; DON'T remove   */
	    		d21d022_unbind(ma_bind_id);
        		DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, 
				return_code->errvalue));
	    		return(D2_NOERROR);
			}

		/*check subset                        */
		if (subset == OBJECT)
			{                       
	   		pb.d21_5base_obj = object; 
	   		pb.d21_5subset = D2_BASE_OBJECT ;
			}
		else
			{
	   		pb.d21_5base_obj = (object == NULL) ? 
					   (D2_name_string) NULL : object;
	   		pb.d21_5subset = D2_WHOLE_SUBTREE ;
			}
				
		sprintf(filename, "%s%s%s%s%d", dcelocal_path,
			D27_CLIENT_DIR, D27_DUA_DIR, D21_SEARCH, getpid());

	   	pb.d21_5pbhead = &head ;
	   	pb.d21_5c_a = &c_arg ;
	   	pb.d21_5bind_id = ma_bind_id ;
	   	pb.d21_5srch_al = FALSE;
	   	pb.d21_5f_pattern = (D2_Fi_filter *) &filter[0];
	      	filter[0].d2_fi_opmod = D2_OR;
	      	filter[0].d2_fi_v.fi_and_or.d2_fi_no_fi = 2;
	      	filter[0].d2_fi_v.fi_and_or.d2_fi_filter = &filter[1];
	      	filter[1].d2_fi_opmod = D2_ITEM;
		p_item = &filter[1].d2_fi_v.d2_fi_item;
		p_item->d2_fi_match = D2_EQUAL;
		p_item->d2_fi_type.d2_type = (Ob_id_string)D2_MASTER_KNOWLEDGE ;
		p_item->d2_fi_type.d2_typ_len = sizeof( D2_MASTER_KNOWLEDGE) -1;
		p_val = &p_item->d2_fi_val.d2_fi_att_val;
		p_val->d2_a_v_len = strlen(local_dsa) + 1;
		p_val->d2_a_rep = D2_DISTNAME;
		p_val->d2_at_value.d2_oct_a_v = (Octet_string)local_dsa;
	      	filter[2].d2_fi_opmod = D2_ITEM;
		p_item = &filter[2].d2_fi_v.d2_fi_item;
		p_item->d2_fi_match = D2_PRESENT;
		p_item->d2_fi_type.d2_type = (Ob_id_string)D2_ALIASED_OBJECT;
		p_item->d2_fi_type.d2_typ_len = sizeof( D2_ALIASED_OBJECT) - 1;
		p_val = &p_item->d2_fi_val.d2_fi_att_val;
		p_val->d2_a_v_len = 0;
		p_val->d2_at_value.d2_oct_a_v = NULL;

	   	pb.d21_5r_info = &reqinfo ;
	   	pb.d21_5file = filename ;
	   	pb.d21_5err = (D2_error *)NULL;
	   	pb.d21_5cres = &c_res ; 

	   	if(d21_5_search (&pb) == D2_NOERROR)
	   		{                     
			/* remove all objects              */

			pbrem.d21_8pbhead = &head ;
			pbrem.d21_8c_a = &c_arg ;
			pbrem.d21_8bind_id = ma_bind_id ;
			pbrem.d21_8err = (D2_error *)NULL ;

			strcpy(d20_m_dsa, local_dsa);

	      		if(d21d021_rem_shadows(filename, return_code, &pbrem) 
								!= D2_NOERROR)
	      			{  
				d21d022_unbind(ma_bind_id);
		 		unlink(filename);
        			DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, 
					return_code->errvalue));
		 		return(D2_ERROR);
	      			}

		 	unlink(filename);
	      		}
	   	else 
			{                  
			/* set error code          */
	       		return_code->retcode  = PERMERR;
	       		return_code->errclass = SYSERR;
	       		return_code->errvalue = DS_SEARCH_ERR;
	       		d21d022_unbind(ma_bind_id);
	       		unlink(filename);
        		DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, 
				return_code->errvalue));
	       		return(D2_ERROR);
	   		}
		}
    	else
    		{   
        	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, 
			return_code->errvalue));
		return(D2_ERROR);
    		}
	}

d21d022_unbind(ma_bind_id);

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
return(D2_NOERROR);
}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21d021_rem_shadows()                                 */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        14.01.88                                              */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will scan the search result file        */
/*              remove all shadows in a loop.                         */
/*              If a shadow has subordinates it is okay that the      */
/*              remove fails.                                         */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char            *filename;                            */
/*              D21_p8          *pbrem;                               */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              Errcode         return_code;                          */
/*								      */
/* RETURNVALUE:							      */
/*              D2_NOERROR:     no errors                             */
/*              D2_ERROR:       bind failed                           */
/*								      */
/*exoff ***************************************************************/

static d2_ret_val d21d021_rem_shadows(
  char * filename,
  Errcode * return_code,
  D21_p8 * pbrem)
{                               /*  d21d021_rem_shadows               */
 static char function[] = "d21d021_rem_shadows";
 signed32     no_bytes = 0;             /* actual number of bytes */
				        /* read in search file    */

 D20_dn_off   dn;                       /* DN-info                */
 D20_dn_off   mk;                       /* master knowledge info  */

 D20_dns      ext_dn;                   /* external DN-list       */
 D20_dns      int_dn;                   /* internal DN-List       */

 signed32     attr_len;
 int   fd;

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

dn.poff = mk.poff = NULL;
dn.off = mk.off = NULL;
int_dn.names = ext_dn.names = NULL;

int_dn.max_anz = D20_MORE;
ext_dn.max_anz = D20_DN_SIZE;
int_dn.anz = ext_dn.anz = 0;

dn.no = mk.no = 0;
dn.max_no = D20_DN_NO;
mk.max_no = D20_INT_DN_NO;

/*  open of file fails                */
if ((fd = open(filename, O_RDONLY)) == -1)
	{
  	return_code->retcode  = PERMERR;
  	return_code->errclass = SYSERR;
  	return_code->errvalue = DS_DEL_ERR;
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%s%d"),GDS_S_GENERAL,
		svc_c_debug1, GDS_S_FOPEN_ERR, filename, "r", errno));
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
  	return(D2_ERROR);
	}
else
	/*  no memory for offsets             */
      	if ((dn.poff = (D20_offset **) D27_MALLOC(svc_c_debug1, 
		(size_t) dn.max_no * sizeof(char *))) == NULL ||
            (dn.off = (D20_offset *) D27_MALLOC(svc_c_debug1, 
		(size_t) dn.max_no * sizeof(D20_offset))) == NULL ||
            (mk.poff = (D20_offset **) D27_MALLOC(svc_c_debug1, 
		(size_t) mk.max_no * sizeof(char *))) == NULL ||
            (mk.off = (D20_offset *) D27_MALLOC(svc_c_debug1, 
		(size_t) mk.max_no * sizeof(D20_offset))) == NULL ||
            (ext_dn.names = D27_MALLOC(svc_c_debug1, 
		(size_t) ext_dn.max_anz)) == NULL ||
            (int_dn.names = D27_MALLOC(svc_c_debug1, (size_t) int_dn.max_anz)) 
		== NULL)
		{
		/*  return(D2_ERROR)                  */
  		return_code->retcode  = PERMERR;
  		return_code->errclass = SYSERR;
  		return_code->errvalue = DS_DEL_ERR;
     		d20_111_free((char *)dn.poff, (char *)dn.off, (char *)mk.poff,
			     (char *)mk.off, ext_dn.names, 
		     	     int_dn.names, (char *)NULL, (char *)NULL, 
		     	     (char *)NULL, (char *)NULL);
  		close(fd);
        	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, 
			return_code->errvalue));
  		return(D2_ERROR);
		}

attr_len = 0L;

/*  operation fails                   */
if (d20_106_get_dn_offsets(FALSE, fd, &ext_dn, &int_dn, &dn, &mk, &no_bytes, 
			   &attr_len))
	{
	/*  return(D2_ERROR)                  */
   	return_code->retcode  = PERMERR;
   	return_code->errclass = SYSERR;
   	return_code->errvalue = DS_DEL_ERR;
     	d20_111_free((char *)dn.poff, (char *)dn.off, (char *)mk.poff,
		     (char *)mk.off, ext_dn.names, 
		     int_dn.names, (char *)NULL, (char *)NULL, 
		     (char *)NULL, (char *)NULL);
   	close(fd);
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
   	return(D2_ERROR);
	}

d20_beg1 = d20_beg2 = ext_dn.names;

/* sort entry dns                     */
qsort(dn.poff, (size_t) dn.no, (size_t) sizeof(char *),
      (int (*)(const void *, const void *)) d20_101_compare_dn);

/* set install flag for alias objects.	*/
if (d20_099_set_install_alias(pbrem->d21_8bind_id, dn.poff, dn.no, ext_dn.names,
				 filename, attr_len) == D2_ERROR)
	{
	/*  return(D2_ERROR)                  */
   	return_code->retcode  = PERMERR;
   	return_code->errclass = SYSERR;
   	return_code->errvalue = DS_DEL_ERR;
     	d20_111_free((char *)dn.poff, (char *)dn.off, (char *)mk.poff,
		     (char *)mk.off, ext_dn.names, 
		     int_dn.names, (char *)NULL, (char *)NULL, 
		     (char *)NULL, (char *)NULL);
   	close(fd);
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
    	return(D2_ERROR);
	}


if (d21d023_handle_dns(dn.poff, dn.no, pbrem) != D2_NOERROR)
	{
     	return_code->retcode  = PERMERR;
     	return_code->errclass = SYSERR;
     	return_code->errvalue = DS_DEL_ERR;
     	d20_111_free((char *)dn.poff, (char *)dn.off, (char *)mk.poff,
		     (char *)mk.off, ext_dn.names, 
		     int_dn.names, (char *)NULL, (char *)NULL, 
		     (char *)NULL, (char *)NULL);
     	close(fd);
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
     	return(D2_ERROR);
	}


d20_111_free((char *)mk.poff, (char *)mk.off, int_dn.names, (char *)NULL,
	     (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL,
	     (char *)NULL, (char *)NULL);

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
return(D2_NOERROR);
}                               /*  d21d021_rem_shadows               */

/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21d023_handle_dns()                                  */
/*								      */
/* AUTHOR:      Volpers(D AP 11)                                      */
/* DATE:        16.03.89                                              */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will remove all shadows                 */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_offset **dn;        pointer list to DNs in DSA1   */
/*              signed32      dn_no;    number of DNs in DSA1         */
/*              D21_p8       *pbrem;                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*              D2_NOERROR: operation is ok                           */
/*              D2_ERROR:   operation fails                           */
/*								      */
/*inoff ***************************************************************/

static d2_ret_val d21d023_handle_dns(
  D20_offset ** dn,
  signed32 dn_no,
  D21_p8 * pbrem)
{
 D20_offset **dn_max = dn + dn_no;
 D20_offset **dn_sav = dn;

dn = dn_max - 1;

/*  DNs in second DSA exist           */
while (dn >= dn_sav)
	{
	if (!((*dn)->d20_state & D21_ALIAS_OBJ) || (((*dn)->d20_state & D21_ALIAS_OBJ) && ((*dn)->d20_state & D20_INSTALL)))
		{
		/* delete shadow                      */
		pbrem->d21_8entry = (D2_name_string) d20_beg2 + 
							(*dn)->d20_mem_off;

		if (d21_8_remove_object(pbrem) != D2_NOERROR)
			if (pbrem->d21_8pbhead->d2_errvalue != D2_ORPHAN)  
				/* no subordinates exist  */
				return(D2_ERROR);
		}

	/*  take next DN from DSA2            */
  	dn--;
	}

return(D2_NOERROR);
} /* end of d21d023_handle_dns */

