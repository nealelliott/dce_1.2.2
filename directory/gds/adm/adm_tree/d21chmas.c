/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21chmas.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:54  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:15:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/mothra_work/1  1995/07/07  23:00 UTC  dtruong
 * 	Correct uninitialize variable
 * 	[1995/12/08  14:51:15  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:21  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:09:01  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:51  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:06:52  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:55  marrek
 * 	Update.
 * 	[1994/02/09  16:14:29  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:23:57  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:36:28  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:21:54  marrek]
 * 
 * Revision 1.1.4.5  1993/01/29  08:31:19  marrek
 * 	Add D2_DREFALIAS.
 * 	[1993/01/28  10:19:35  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  17:48:16  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:02:32  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/18  08:25:28  marrek
 * 	Various bug fixes for mask handling found at SNI
 * 	[1992/12/17  16:24:26  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  14:20:31  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:34:29  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:48:00  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:12:51  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21chmas.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:52 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/
 
/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : d21chmas.o                                          */
/*                                                                    */
/* AUTHOR       : N. Serra, D525 (CDS)                                */
/* DATE         : 23.10.89                                            */
/*                                                                    */
/* KOMPONENTE   : DS                                                  */
/*                                                                    */
/* DOK.-NR.     : DS-Design-Spez.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 23.10.89| Erstellung                     | hv |         */
/*                                                                    */
/*datoff **************************************************************/
 
/* Includes */

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d21iapl.h>
#include <d21.h>
#include <d2delupd.h>
#include <d2dump.h>
#include <d20proto.h>


/* Defines */

#define D2_1_TRUE            1
#define D2_2_TRUE            2
#define CONVERR(value) (value % 16 + '0') /* Conversion short -> char */
 

/* Variables */

extern  D2_pbhead   *d20_head;
 
  char       *d20_beg1;             /* start address of DN-strings */
  char       *d20_beg2;             /* start address of DN-strings */
  signed32    min_pos, max_pos;
  d2_ret_val  ret_value;            /* return value for the        */

static  struct  d21_bind_info bind_info;
 
 
/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21_13_chmas()                                         */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will change the master knowledge attribute for  */
/*      a subtree from an old value to a new value. Only master       */
/*      entries will be changed, i. e. the changes will take place    */
/*      only on the old master DSA.                                   */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed16      bind_id;          session identification        */
/*      signed16      version;          version of interface          */
/*      Name_string   subtree_root;     base object of the subtree    */
/*                                      for which the master knowled- */
/*                                      ge will be changed            */
/*      Name_string   old_master;       old master DSA for the        */
/*                                      choosen subtree               */
/*      Name_string   new_master;       new master DSA for the        */
/*                                      choosen subtree               */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Name_string **not_changed;      list of objects which were    */
/*                                      not changed                   */
/*      Errcode      *return_code;      return code of operation      */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  : function terminates without error               */
/*      D2_ERROR    : function terminates with error                  */
/*                                                                    */
/*exoff ***************************************************************/
 
 signed32 d21_13_chmas(
   signed16      bind_id,
   Errcode      *return_code,
   signed16      version,
   Name_string   subtree_root,
   Name_string   old_master,
   Name_string   new_master,
   Name_string **not_changed)
 
{
   signed16     dir_id;
   signed16     new_bind_id;
   char         bind_dsa[D2_DNL_MAX + 1];   /* name of the bind DSA       */
   char         filename[D27_LFILE_NAME];   /* name of the SEARCH file    */
   D21_p5       pb;                         /* parameter block for SEARCH */
   D21_pC       pbmod;                      /* paremeter block for MODIFY */
   D2_a_modinf  modinf;                     /* info for MODIFY function   */
   D2_a_value   oldnewval[2];               /* info for MODIFY function   */
 
   static   D2_req_info    rinf = {D2_R_VAL_TYP, -1, NULL};
   static   D2_pbhead      head = {D21V022, D2_NO_ERR, D2_NO_ERR, D2_NO_ERR};
   static   D2_c_arg  d2_c_arg;
   D2_c_res c_res;

   char         norm_old_master[D2_DNL_MAX + 1];   
   char         norm_new_master[D2_DNL_MAX + 1];   

Bool		dummy;
D2_pbhead	dummy_pbhead;
 
/*
d21d009_errlog("ENTRY d21_13_chmas", NULL);
*/

/*
d27_009_trace_dn("  Subtree:", (char *)subtree_root);
*/
/*
d27_009_trace_dn("  Old Master:", (char *)old_master);
*/
/*
d27_009_trace_dn("  New Master:", (char *)new_master);
*/

/* check version, bind-id */
if (d21t009_param_check(bind_id, return_code, version, &dir_id) == D2_ERROR) 
	{
	/*
      	d21d009_errlog("EXIT  d21_13_chmas", return_code);
	*/
      	return(D2_ERROR);
   	}
 
d20_head = &head;

strcpy(norm_old_master, (char *)old_master);
d20_norm(norm_old_master, strlen(norm_old_master));

strcpy(norm_new_master, (char *)new_master);
d20_norm(norm_new_master, strlen(norm_new_master));

/* old_master is same as new_master.	*/
if (!strcmp(norm_old_master, norm_new_master)) 
	{
	/*
      	d21d009_errlog("EXIT  d21_13_chmas", return_code);
	*/
      	return(D2_NOERROR);
   	}

new_bind_id = bind_id;

if (d21_bh8_get_bind_info(bind_id, &bind_info, &dummy, &dummy_pbhead, FALSE) 
       				!= D2_ERROR) 
	{
      	strcpy(bind_dsa, (char *)bind_info.d21_b_name_dsa);
   	}
else 
	{
      	return_code->retcode  = CALLERR;
      	return_code->errclass = PARERR;
      	return_code->errvalue = DS_NO_BIND;
	/*
      	d21d009_errlog("EXIT  d21_13_chmas", return_code);
	*/
      	return(D2_ERROR);
   	}
 
/* bind to old_master DSA.	*/
if (d21t047_bind_dsa(bind_dsa, (char *) old_master, &new_bind_id, dir_id, 
                         &d2_c_arg) == D2_ERROR) 
	{
      	return_code->retcode  = TEMPERR;
      	return_code->errclass = SYSERR;
      	return_code->errvalue = DS_NO_RESPONSE;
	/*
	d21d009_errlog("EXIT  d21_13_chmas", return_code);
	*/
	return(D2_ERROR);
   	}
 
sprintf(filename, "%s%d", "CHMAS", getpid());    /* subtree SEARCH file  */
 
pb.d21_5pbhead = &head;
pb.d21_5c_a = &d2_c_arg;
	d2_c_arg.d2_serv_cntrls = D2_LOCALSCOPE | D2_NOREFERRAL | D2_USEDSA | 
			  D2_DONT_STORE | D2_PREF_ADM_FUN | D2_DREFALIAS;
   	d2_c_arg.d2_priority    = D2_PRIO_LOW;
   	d2_c_arg.d2_time_limit  = D2_T_UNLIMITED;
   	d2_c_arg.d2_size_limit  = D2_S_UNLIMITED;
pb.d21_5bind_id = new_bind_id; 
pb.d21_5base_obj = (subtree_root == NULL) ? (D2_name_string)NULL :
                      subtree_root;
pb.d21_5subset = D2_WHOLE_SUBTREE;
pb.d21_5srch_al = FALSE;
pb.d21_5f_pattern = NULL;
pb.d21_5r_info = &rinf;
pb.d21_5file = filename;
pb.d21_5err = (D2_error *)NULL;
pb.d21_5cres = &c_res ; 

pbmod.d21_Cpbhead = &head;
pbmod.d21_Cc_a = &d2_c_arg;
   d2_c_arg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
   d2_c_arg.d2_al_rdn = 0;

pbmod.d21_Cmod = &modinf;

modinf.d2_m_next = (D2_a_modinf *)NULL;
modinf.d2_m_mod = D2_REP_AV;
modinf.d2_m_attr.d2_a_next = (D2_a_info *)NULL;
modinf.d2_m_attr.d2_a_type.d2_typ_len = sizeof(D2_MASTER_KNOWLEDGE)-1;
modinf.d2_m_attr.d2_a_type.d2_type = (Ob_id_string) D2_MASTER_KNOWLEDGE;
modinf.d2_m_attr.d2_a_val = &oldnewval[0];
modinf.d2_m_attr.d2_a_no_val = 2;

oldnewval[0].d2_a_v_len = strlen((char *)old_master) + 1;
oldnewval[0].d2_at_value.d2_obj_name_a_v = old_master;
oldnewval[0].d2_a_rep = (unsigned16) d27_401_syntax_to_rep(
				(signed16)D2_DISTNAME);
oldnewval[1].d2_a_v_len = strlen((char *)new_master) + 1;
oldnewval[1].d2_a_rep = (unsigned16) d27_401_syntax_to_rep(
				(signed16)D2_DISTNAME);
oldnewval[1].d2_at_value.d2_obj_name_a_v = new_master;

/* search for subtree on old_master DSA.	*/
if (d21_5_search(&pb) == D2_ERROR) 
	{    
      	if (pb.d21_5pbhead->d2_errvalue == D2_NFOUND) 
		{
	 	return_code->retcode  = CALLERR;
	 	return_code->errclass = PARERR;
         	return_code->errvalue = DS_NFOUND;
      		}
      	else 
		{
	 	return_code->retcode  = PERMERR;
	 	return_code->errclass = SYSERR;
         	return_code->errvalue = DS_SEARCH_ERR;
      		}

	/*
      	d21d009_errlog("EXIT  d21_13_chmas", return_code);
	*/
      	unlink(filename);

      	if (new_bind_id != bind_id) 
         	d20__unbind(new_bind_id);

      	return(D2_ERROR);
   	}
 
ret_value = d21t050_change_master(filename, return_code, &pb, &pbmod,
			  not_changed, (Name_string) norm_old_master, 
                          (Name_string) norm_new_master, bind_dsa, dir_id,
			  bind_id, &new_bind_id);
 
unlink(filename);

if (new_bind_id != bind_id) 
   	d20__unbind(new_bind_id);

/*
d21d009_errlog("EXIT  d21_13_chmas", return_code);
*/
return((signed32) ret_value);

}


 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE    :   C-FUNCTION                                             */
/*                                                                    */
/* NAME    :   d21t050_change_master()                                */
/*                                                                    */
/* AUTHOR  :   N. Serra, D 525 (CDS)                                  */
/* DATE    :   25.10.89                                               */
/*                                                                    */
/* SYNTAX  :   signed16 d21t050_change_master(filename,		      */
/*                      return_code, pb, pbmod, not_changed,          */
/*                      old_master, new_master, bind_dsa, dir_id,     */
/*                      bind_id, new_bind_id)                	      */
/*             char         *filename;                                */
/*             Errcode      *return_code;                             */
/*             D21_p5       *pb;                                      */
/*             D21_pC       *pbmod;                                   */
/*             Name_string **not_changed;                             */
/*             Name_string   old_master;                              */
/*             Name_string   new_master;                              */
/*             char         *bind_dsa;                                */
/*             signed16      dir_id;                                  */
/*             signed16      bind_id;                                 */
/*             signed16     *new_bind_id;                             */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*       This function will scan the search result files and it will  */
/*       try to change the master of all the objects that have the    */
/*       old one.                                                     */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*       char         *filename;       name of the "search" file of   */
/*                                     the subtree                    */
/*       D21_p5       *pb;             parameter block for SEARCH     */
/*       D21_pC       *pbmod;          paremeter block for MODIFY     */
/*                                     the root of the subtree        */
/*       Name_string   old_master;     master to be changed           */
/*       Name_string   new_master;     new master                     */
/*       char         *bind_dsa;       name of the bind DSA           */
/*       signed16      dir_id;         directory identification       */
/*       signed16      bind_id;        original bind identification   */
/*       signed16     *new_bind_id;    bind identification            */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*       Errcode      *return_code;    return code of operation       */
/*       Name_string **not_changed;    list of objects which were not */
/*                                     changed                        */
/*                                                                    */
/* RETURN-VALUE:                                                      */
/*                D2_NOERROR : no errors                              */
/*                D2_ERROR   : function failed                        */
/*                                                                    */
/*exoff ***************************************************************/
 
 signed16 d21t050_change_master(
   char         *filename,
   Errcode      *return_code,
   D21_p5       *pb,
   D21_pC       *pbmod,
   Name_string **not_changed,
   Name_string   old_master,
   Name_string   new_master,
   char         *bind_dsa,
   signed16      dir_id,
   signed16      bind_id,
   signed16     *new_bind_id)
 
{
   D20_dn_off      dn, dn1;                       /* DN-info                */
   D20_dn_off      mk, mk1;                       /* master knowledge info  */
   D20_dns         ext_dn, ext_dn1;
   D20_dns         int_dn, int_dn1;
   D20_offset     *obj_aux;
   char           *at_mem;
   char           *mk_attr;
   signed16        mk_len;
   int             fd, fd1;

   signed32	  attr_len, attr_len1;
   char		 *at_val_area;

   signed32       cc1, cc2, cc3;
   signed32       cc1_max, cc2_max, cc3_max;
   Bool           *b_notchm;
   signed16       *b_mustex;
   char            filename1[D27_LFILE_NAME];
   char           *object, *object1;
 
   static   D2_req_info    rinf1 = {D2_R_VAL_TYP, -1, NULL};
   D2_c_res c_res;
   D2_a_type MK;
   char           auxobj[D2_DNL_MAX+1];

char	norm_mk_attr[D2_DNL_MAX + 1];
char	norm_scheme[D2_DNL_MAX + 1];

strcpy(norm_scheme, D2_NAME_SCHEME);
d20_norm(norm_scheme, strlen(norm_scheme));
 
MK.d2_typ_len = sizeof(D2_MASTER_KNOWLEDGE) - 1;
MK.d2_type = (Ob_id_string)D2_MASTER_KNOWLEDGE;

*not_changed = NULL;
ret_value = D2_NOERROR;
 
/* read the files containing the DNs of DSA1 (old_master) */
if (d21t032_get_search_file(&fd, filename, return_code, &dn, &mk, &ext_dn, 
						&int_dn, &attr_len) == D2_ERROR)
      return(D2_ERROR);


/* sort entry dns                  */
d20_beg1 = d20_beg2 = ext_dn.names;
qsort(dn.poff, (size_t) dn.no, (size_t) sizeof(String),
      (int (*)(const void *, const void *)) d20_101_compare_dn);

attr_len += D2_ALIGN(D2_DNL_MAX);
if (attr_len < D20_BUFSIZ)
	attr_len = D20_BUFSIZ;

/* allocate memory for control arrays and get memory for the attributes  */
if (((b_notchm = (Bool *)malloc((size_t)(dn.no) *
                                   sizeof(Bool))) == (Bool *)NULL) ||
    ((b_mustex = (signed16 *)malloc((size_t)(dn.no) *
                                  sizeof(signed16))) == (signed16 *)NULL) ||
    ((at_mem = malloc(attr_len)) == NULL))
	{
      	return_code->retcode  = PERMERR;
      	return_code->errclass = SYSERR;
      	return_code->errvalue = DS_ALLOC_ERROR;
      	close(fd);
      	d20_111_free((char *) dn.poff, (char *) dn.off, (char *) mk.poff,
		     (char *) mk.off, ext_dn.names, int_dn.names,
		     (char *) b_notchm,(char *) b_mustex, at_mem, (char *)NULL);
      	return(D2_ERROR);
   	}

#ifndef __hpux_13317CHFts
/* check if root of subtree is an alias.        */
if (pb->d21_5base_obj != (D2_name_string)NULL)
        {
        /* check if first object in the sorted list is an alias.        */
        if (((*dn.poff)->d20_state & D21_ALIAS_OBJ) != 0)
                {
                return_code->retcode  = PERMERR;
                return_code->errclass = SYSERR;
                return_code->errvalue = D2_ON_ALIAS_NOT_ALLOWED;
                close(fd);
                d20_111_free((char *) dn.poff, (char *) dn.off,(char *) mk.poff,
                     (char *) mk.off, ext_dn.names, int_dn.names,
                     (char *) b_notchm,(char *) b_mustex, at_mem, (char *)NULL);
                return(D2_ERROR);
                }
        }
else
        {
        /* alias objects could be directly under root or under other objects. */
        }
#endif
if ((at_val_area = malloc(attr_len + (D2_AT_MAX * max_comp_attr()))) == NULL)
	{
      	return_code->retcode  = PERMERR;
      	return_code->errclass = SYSERR;
      	return_code->errvalue = DS_ALLOC_ERROR;
      	close(fd);
      	d20_111_free((char *) dn.poff, (char *) dn.off, (char *) mk.poff,
		     (char *) mk.off, ext_dn.names, int_dn.names,
		     (char *) b_notchm,(char *) b_mustex, at_mem, (char *)NULL);
      	return(D2_ERROR);
	}

/* initialize the arrays            */
for (cc1 = 0, cc1_max = dn.no; cc1 < cc1_max; cc1++) 
      	*(b_notchm + cc1) = *(b_mustex + cc1) = FALSE;
 
min_pos = max_pos = 0;

/* get the MK attribute of the objects and check if it must be changed */
for (cc1 = 0, cc1_max = dn.no; cc1 < cc1_max; cc1++) 
	{
	if (((*(dn.poff + cc1))->d20_state & D21_ALIAS_OBJ) == 0) 
		{
		/* object is not an alias.	*/
		if (d21t008_get_attr(fd, *(dn.poff + cc1), at_mem, 
                              &min_pos, &max_pos, MK, 
                              &mk_attr, &mk_len, d20_beg1) == D2_ERROR) 
			{
            		return_code->retcode  = PERMERR;
            		return_code->errclass = SYSERR;
            		return_code->errvalue = DS_SEARCH_ERR;
			close(fd);
      			d20_111_free((char *) dn.poff, (char *) dn.off,
			  (char *) mk.poff, (char *) mk.off, ext_dn.names,
			  int_dn.names, (char *) b_notchm, (char *) b_mustex, 
			  at_mem, at_val_area);
            		return(D2_ERROR);
         		}

		/* object is a master object.	*/
		strcpy(norm_mk_attr, mk_attr);
		d20_norm(norm_mk_attr, strlen(mk_attr));

      		if (!strcmp(norm_mk_attr, (char *)old_master)) 
			{
         		*(b_notchm + cc1) = TRUE;
	 		*(b_mustex + cc1) = D2_2_TRUE;
      			}
		}
	else
		{
		/* object is an alias.			*/
		/* MK is to be changed, if it is to be changed for the parent.*/
	
		/* get the parent object name of the alias object.	*/
            	if (d21t033_parent_object_name(d20_beg1 + (*(dn.poff + cc1))
					       ->d20_mem_off, auxobj) != FALSE)
			{
			/* look for parent in the search result 	*/
			/* (already checked). 				*/
			for (cc2 = 0, cc2_max = cc1; cc2 < cc1; cc2++) 
                  		if (strcmp(d20_beg1 + (*(dn.poff + cc2))
						->d20_mem_off, auxobj) == 0)
					break;

			if (cc2 != cc1)
				{
				/* parent of alias found.	*/
         			if (*(b_notchm + cc2) == TRUE)
					{
					/* parent's MK is to changed, 	*/
					/* so change for alias also.	*/
         				*(b_notchm + cc1) = TRUE;
	 				*(b_mustex + cc1) = D2_2_TRUE;
					}
				}
			else
				{
				/* parent of alias not found in search result.*/
            			return_code->retcode  = PERMERR;
            			return_code->errclass = SYSERR;
            			return_code->errvalue = DS_SEARCH_ERR;
				close(fd);
      				d20_111_free((char *) dn.poff, (char *) dn.off,
				  (char *) mk.poff, (char *) mk.off, 
				  ext_dn.names, int_dn.names, (char *) b_notchm,
				  (char *) b_mustex, at_mem, at_val_area);
            			return(D2_ERROR);
				}
			}
		else
			{
			/* alias is directly under root.		 */
			/* Schema object should be in the search result. */

			/* look for the schema object in search result.	*/
			for (cc2 = 0, cc2_max = dn.no; cc2 < dn.no; cc2++) 
                  		if (strcmp(d20_beg1 + (*(dn.poff + cc2))->
					   d20_mem_off, norm_scheme) == 0)
					break;

			if (cc2 == dn.no)
				{
				/* schema object not found in search result. */
            			return_code->retcode  = PERMERR;
            			return_code->errclass = SYSERR;
            			return_code->errvalue = DS_SEARCH_ERR;
				close(fd);
      				d20_111_free((char *) dn.poff, (char *) dn.off,
				  (char *) mk.poff, (char *) mk.off, 
				  ext_dn.names, int_dn.names, (char *) b_notchm,
				  (char *) b_mustex, at_mem, at_val_area);
            			return(D2_ERROR);
				}
			else
				{
				if (d21t008_get_attr(fd, *(dn.poff + cc2), 
				    at_mem, &min_pos, &max_pos, MK, &mk_attr, 
				    &mk_len, d20_beg1) == D2_ERROR) 
					{
            				return_code->retcode  = PERMERR;
            				return_code->errclass = SYSERR;
            				return_code->errvalue = DS_SEARCH_ERR;
					close(fd);
      					d20_111_free((char *) dn.poff,
					(char *) dn.off, (char *) mk.poff, 
					(char *) mk.off, ext_dn.names,
					int_dn.names, (char *) b_notchm,
					(char *) b_mustex, at_mem, at_val_area);
            				return(D2_ERROR);
         				}

				/* object is a master object.	*/
				strcpy(norm_mk_attr, mk_attr);
				d20_norm(norm_mk_attr, strlen(mk_attr));

      				if (!strcmp(norm_mk_attr, (char *)old_master)) 
					{
         				*(b_notchm + cc1) = TRUE;
	 				*(b_mustex + cc1) = D2_2_TRUE;
      					}
				}
			} /* alias is under root.	*/
		} /* object is an alias.	*/
   	}

/* bind to new master DSA.	*/
if ((ret_value = d21t047_bind_dsa(bind_dsa, (char *) new_master, 
			new_bind_id, dir_id, pb->d21_5c_a)) == D2_ERROR)
	{ 
      	return_code->retcode  = TEMPERR;
      	return_code->errclass = SYSERR;
      	return_code->errvalue = DS_NO_RESPONSE;
	close(fd);
      	d20_111_free((char *) dn.poff, (char *) dn.off, (char *) mk.poff,
		     (char *) mk.off, ext_dn.names, int_dn.names,
		     (char *) b_notchm, (char *) b_mustex, at_mem, at_val_area);
	return(D2_ERROR);
      	}
 
sprintf(filename1, "%s%d_1", "CHMAS", getpid());

/* search subtree on new_master DSA.	*/
pb->d21_5bind_id = *new_bind_id; 
pb->d21_5subset = D2_WHOLE_SUBTREE;
pb->d21_5r_info = &rinf1;
pb->d21_5file = filename1;
pb->d21_5err = (D2_error *)NULL;
pb->d21_5cres = &c_res ; 

   
if (d21_5_search(pb) == D2_ERROR) 
	{
	return_code->retcode  = PERMERR;
        return_code->errclass = SYSERR;
        return_code->errvalue = DS_SEARCH_ERR;
	close(fd);
        unlink(filename1);
      	d20_111_free((char *) dn.poff, (char *) dn.off, (char *) mk.poff,
		     (char *) mk.off, ext_dn.names, int_dn.names,
		     (char *) b_notchm, (char *) b_mustex, at_mem, at_val_area);
        return(D2_ERROR);
      	}

/* read the files containing the DNs of DSA2 (new_master) */

if (d21t032_get_search_file(&fd1, filename1, return_code, &dn1, &mk1, 
			    &ext_dn1, &int_dn1, &attr_len1) == D2_ERROR)
	{
      	close(fd);
      	unlink(filename1);
      	d20_111_free((char *) dn.poff, (char *) dn.off, (char *) mk.poff,
		     (char *) mk.off, ext_dn.names, int_dn.names,
		     (char *) b_notchm, (char *) b_mustex, at_mem, at_val_area); 
      	d20_111_free((char *) dn1.poff, (char *) dn1.off, (char *) mk1.poff,
		     (char *) mk1.off, ext_dn1.names, int_dn1.names,
		     (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL); 
      	return(D2_ERROR);
	}
 
d20_beg1 = d20_beg2 = ext_dn1.names;
min_pos = max_pos = 0;

qsort(dn1.poff, (size_t) dn1.no, (size_t) sizeof(String),
      (int (*)(const void *, const void *)) d20_101_compare_dn);

d20_beg1 = d20_beg2 = ext_dn.names;

/* check the objects that must exist in DSA2 (new_master) */
for (cc1 = 0, cc1_max = dn.no, cc2 = 0; cc1 < cc1_max; cc1++) 
	{
      	if (*(b_notchm + cc1) == TRUE) 
		{
	     	/* object must exist in DSA2 */
         	object = d20_beg1 + (*(dn.poff + cc1))->d20_mem_off;

	 	for (cc2_max = dn1.no; cc2 < cc2_max; cc2++) 
			{
			/* search in DSA2 */
            		object1 = ext_dn1.names + 
				(*(dn1.poff + cc2))->d20_mem_off;

            		if (!strcmp(object, object1)) 
				/* the checked object already exists */
	       			break;  
         		}

         	if (cc2 == dn1.no) 
			{
			/* error: the checked object does not exist */
            		return_code->retcode  = PERMERR;
            		return_code->errclass = SYSERR;
	    		return_code->errvalue = DS_NO_NEW_MASTER;
			close(fd);
			close(fd1);
			unlink(filename1);
      			d20_111_free((char *) dn.poff, (char *) dn.off,
			  (char *) mk.poff, (char *) mk.off, ext_dn.names,
			  int_dn.names, (char *) b_notchm, (char *) b_mustex, 
			  at_mem, at_val_area);
      			d20_111_free((char *) dn1.poff, (char *) dn1.off,
			  (char *) mk1.poff, (char *) mk1.off, ext_dn1.names,
			  int_dn1.names, (char *)NULL, (char *)NULL,
			  (char *)NULL, (char *)NULL); 
            		return(D2_ERROR);
         		}

		/* object exists as shadow.	*/
           	*(b_mustex + cc1) = D2_1_TRUE;

		/*look for the children of old master entries in DSA2 */
	    	for (cc3 = cc1 + 1, cc3_max = dn.no; cc3 < cc3_max; cc3++) 
               		if (*(b_mustex + cc3) == FALSE) 
			/* shadow object.	*/
                  		if (d21t051_is_parent(object,
				d20_beg1 + (*(dn.poff + cc3))->d20_mem_off))
				/* child is a mandatory shadow.		*/
                     			*(b_mustex + cc3) = D2_2_TRUE;
      		}
   	}
 
/* add the objects that must exist in DSA2 (children of old master */
/* entries) 							   */

for (cc1 = 1, cc1_max = dn.no, cc2 = 1; cc1 < cc1_max; cc1++) 
	{
	/* child must exist.	*/
       	if (*(b_mustex + cc1) == D2_2_TRUE) 
		{
		object = d20_beg1 + (*(dn.poff + cc1))->d20_mem_off;

    		cc2_max = dn1.no;

    		while (cc2 < cc2_max) 
			{
			object1 = ext_dn1.names + (*(dn1.poff + cc2))
								->d20_mem_off;

       			if (!strcmp(object, object1)) 
              			break;

       			cc2++;
            		}

		/* child does not exist on new_master DSA.	*/
            	if (cc2 == dn1.no) 
			{
               		cc2 = 1;

                  	obj_aux = *(dn.poff + cc1);

			if (d21t003_add_appnd(fd, obj_aux, at_mem, 
					&min_pos, &max_pos, OLD_ACL, 
					(Name_string)NULL,
					(Name_string)NULL, FALSE, 
					new_master, 0, (char *)NULL,
					at_val_area,CHANGE_MAS) == D2_ERROR)
				{
               			return_code->retcode = 
						CONVERR(d20_head->d2_retcode);
               			return_code->errclass = 
						CONVERR(d20_head->d2_errclass);
               			return_code->errvalue = 
						d20_head->d2_errvalue;
				close(fd);
				close(fd1);
				unlink(filename1);
      				d20_111_free((char *) dn.poff, (char *) dn.off,
				  (char *) mk.poff, (char *) mk.off, 
				  ext_dn.names, int_dn.names, (char *) b_notchm,
				  (char *) b_mustex, at_mem, at_val_area);
      				d20_111_free((char *) dn1.poff, (char *)dn1.off,				  (char *) mk1.poff, (char *) mk1.off,
				  ext_dn1.names, int_dn1.names, (char *)NULL,
				  (char *)NULL, (char *)NULL, (char *)NULL); 
            			return(D2_ERROR);
               			}

         		}
      		}  
	}

pbmod->d21_Cc_a->d2_serv_cntrls &= ~D2_NOCACHE;
pbmod->d21_Cc_a->d2_serv_cntrls |= D2_LOCALSCOPE;

/* modify master knldg. of objects on DSA2, with LOCALSCOPE service controls. */
pbmod->d21_Cbind_id = *new_bind_id;

for (cc1 = 0, cc1_max = dn.no; cc1 < cc1_max; cc1++) 
	{
	if (((*(dn.poff + cc1))->d20_state & D21_ALIAS_OBJ) == 0) 
		{
		/* object is not an alias.	*/
      		if (*(b_notchm + cc1) == TRUE) 
			{
         		pbmod->d21_Centry = (unsigned char*) ext_dn.names + 
						(*(dn.poff + cc1))->d20_mem_off;

         		if (d21_C_modify_attribute(pbmod) == D2_ERROR) 
				{
      				return_code->retcode  = PERMERR;
      				return_code->errclass = SYSERR;
      				return_code->errvalue = DS_MOD_ERR;
				close(fd);
				close(fd1);
				unlink(filename1);
      				d20_111_free((char *) dn.poff, (char *) dn.off,
				  (char *) mk.poff, (char *) mk.off, 
				  ext_dn.names, int_dn.names, (char *) b_notchm,
				  (char *) b_mustex, at_mem, at_val_area);
      				d20_111_free((char *) dn1.poff,(char *) dn1.off,
				  (char *) mk1.poff, (char *) mk1.off,
				  ext_dn1.names, int_dn1.names, (char *)NULL,
				  (char *)NULL, (char *)NULL, (char *)NULL); 
            			return(D2_ERROR);
				}
      			}
		}
	else
		{
		/* Object is an alias. In consistent system, shadow 	  */
		/* is also an alias. Don't try to change the MK, because  */
		/* MK of parent is changed in this process.          	  */
		}
   	}
   
d20_111_free((char *) dn1.poff, (char *) dn1.off, (char *) mk1.poff,
		(char *) mk1.off, ext_dn1.names, int_dn1.names, (char *)NULL,
		(char *)NULL, (char *)NULL, (char *)NULL);
close(fd1);
unlink(filename1);
 
/* bind to old_master DSA.	*/
if ((ret_value = d21t047_bind_dsa(bind_dsa, (char *) old_master, new_bind_id,
				  dir_id, pbmod->d21_Cc_a)) == D2_ERROR)
	{
      	return_code->retcode  = TEMPERR;
      	return_code->errclass = SYSERR;
      	return_code->errvalue = DS_NO_RESPONSE;
	close(fd);
	d20_111_free((char *) dn.poff, (char *) dn.off, (char *) mk.poff,
		(char *) mk.off, ext_dn.names, int_dn.names, (char *) b_notchm,
		(char *) b_mustex, at_mem, at_val_area);
	return(D2_ERROR);
   	}

pbmod->d21_Cc_a->d2_serv_cntrls |= D2_NOCACHE;
pbmod->d21_Cc_a->d2_serv_cntrls &= ~D2_LOCALSCOPE;

/* modify master knldg. of objects on old_master DSA 	*/
/* with master service cntrl.				*/
pbmod->d21_Cbind_id = *new_bind_id;

for (cc1 = 0, cc1_max = dn.no; cc1 < cc1_max; cc1++) 
	{
	if (((*(dn.poff + cc1))->d20_state & D21_ALIAS_OBJ) == 0) 
		{
		/* object is not an alias.	*/
         	if (*(b_notchm + cc1) == TRUE) 
			{
            		pbmod->d21_Centry = (unsigned char *)d20_beg1 + 
				   (*(dn.poff + cc1))->d20_mem_off;

            		if (d21_C_modify_attribute(pbmod) == D2_NOERROR)
               			(*(b_notchm + cc1))--;
			else
				if (pbmod->d21_Cerr->d2_e_type == D2_AT_PROBLEM)
					if (pbmod->d21_Cerr->
						d2_e_problem.d2_e_atproblem->
						d2_ae_prob->d2_ap_prob == 
							D2_ATTRIBUTE_MISSING) 
               					(*(b_notchm + cc1))--;
            				else 
               					ret_value = D2_ERROR;
				else
               				ret_value = D2_ERROR;
         		}
		}
	else
		{
		/* Object is an alias. Don't try to change the MK,  */
		/* because MK of parent would have already been     */
		/* changed by now.   				    */
		}
      	}
 
if (ret_value == D2_ERROR) 
	{    /* error while changing master knowledge */
	d20_beg1 = ext_dn.names;
      	d21t034_not_deleted(dn.no, dn.poff, not_changed, b_notchm); 
      	return_code->retcode  = PERMERR;
      	return_code->errclass = SYSERR;
      	return_code->errvalue = DS_MOD_ERR;
   	}
 
d20_111_free((char *) dn.poff, (char *) dn.off, (char *) mk.poff,
	     (char *) mk.off, ext_dn.names, int_dn.names, (char *) b_notchm,
	     (char *) b_mustex, at_mem, at_val_area);

close(fd);
return(ret_value);

}


 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t051_is_parent()                                   */
/*                                                                    */
/* AUTHOR:      N. Serra, D 525 (CDS)                                 */
/*                                                                    */
/* SYNTAX:      Bool d21t051_is_parent(parent, son)                   */
/*                                                                    */
/*              char    *parent;                                      */
/*              char    *son;                                         */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function checs if "parent" is the parent object  */
/*              of "son".                                             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              char    *parent;    parent object                     */
/*              char    *son;       checked object                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURN-VALUE:                                                      */
/*              TRUE   :  "parent" is the parent object of "son"      */
/*              FALSE  :  "parent" is not the parent object of "son"  */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read-only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/
 
 Bool d21t051_is_parent(
   char *parent,
   char *son)
{
 
for ( ; (*parent != D2_EOS) && (*son != D2_EOS); parent++, son++) 
 	if (*parent != *son) 
         	break;

if ((*parent == D2_EOS) && (*son == D2_RDN_SEP)) 
	{
      	for (son++; *son != D2_EOS; son++) 
         	if (*son == D2_RDN_SEP) 
              		return(FALSE);

        return(TRUE);
   	}

return(FALSE);

}
 
