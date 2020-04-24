/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21delst.c,v $
 * Revision 1.1.719.2  1996/02/18  19:39:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:01  marty]
 *
 * Revision 1.1.719.1  1995/12/08  15:15:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:22  root]
 * 
 * Revision 1.1.717.4  1994/06/21  14:43:25  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:09:31  marrek]
 * 
 * Revision 1.1.717.3  1994/05/10  15:49:20  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:09:44  marrek]
 * 
 * Revision 1.1.717.2  1994/03/23  15:04:56  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:07:24  keutel]
 * 
 * Revision 1.1.717.1  1994/02/22  15:58:01  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:25:01  marrek]
 * 
 * Revision 1.1.715.2  1993/10/14  16:46:20  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:42:57  keutel]
 * 
 * Revision 1.1.715.1  1993/10/13  17:29:23  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  07:13:46  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  09:42:09  marrek
 * 	July 1993 code drop
 * 	[1993/07/30  08:49:43  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:48:33  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:02:52  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:22:42  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:36:05  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:49:01  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:13:23  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21delst.c,v $ $Revision: 1.1.719.2 $ $Date: 1996/02/18 19:39:58 $";
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
/* NAME         : d21delst.o                                          */
/*                                                                    */
/* AUTHOR       : N. Serra, D 525 (CDS)                               */
/* DATE         : 30.08.89                                            */
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
/*      0.1 | 30.08.89| Erstellung                     | hv |         */
/*                                                                    */
/*datoff **************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                     Modulidentification                            */
/*                                                                    */
/**********************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/
 
#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d21iapl.h>
#include <d21.h>
#include <d2delupd.h>
#include <d2dump.h>
#include <d20proto.h>
 
/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/
 
#define D21_SEARCH    "DEL"
#define D20_MEM        50       /* > sizeof(struct D2_ei_info) +      */
                                /*   sizeof(struct D2_a_info)  +      */
                                /*   sizeof(struct D2_a_value)        */
#define NO_REM_ERROR   2
 
/**********************************************************************/
/*                                                                    */
/*                 T Y P E  -  D E F I N I T I O N S                  */
/*                                                                    */
/**********************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                 D E K L A R A T I O N E N      DATEN               */
/*                                                                    */
/**********************************************************************/
 
extern  char    *d20_beg1;              /* start address of DN-strings*/
extern  char    *d20_beg2;              /* start address of DN-strings*/
 
/**********************************************************************/
/*                                                                    */
/*                D E K L A R A T I O N E N      FUNCTIONEN           */
/*                                                                    */
/**********************************************************************/
 

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  signed32 d21_10_delst(bind_id, return_code, version,   */
/*                          subtree_root, serv_cntrls, not_deleted)   */
/*             signed16          bind_id;                             */
/*             Errcode          *return_code;                         */
/*             signed16          version;                             */
/*             Name_string       subtree_root;                        */
/*             signed32          serv_cntrls;                         */
/*             Name_string    **not_deleted;                          */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will delete a subtree from the bind DSA (that   */
/*      is the given object and its subordinates).                    */
/*      If the NOCACHE service control parameter is set, master       */
/*      entries will be deleted. Otherwise, master and/or shadow      */
/*      entries will be deleted from the bind DSA.                    */
/*                                                                    */
/*  INPUT-PARAMETERS:                                                 */
/*      signed16       bind_id;       session identification          */
/*      signed16       version;       version of interface            */
/*      Name_string    subtree_root;  name of the root of the subtree */
/*                                    to be deleted                   */
/*      signed32       serv_cntrls;   service controls to be used for */
/*                                    deleting                        */
/*                                                                    */
/*  OUTPUT-PARAMETERS:                                                */
/*      Name_string  **not_deleted;   list of objects which were not  */
/*                                    deleted                         */
/*      Errcode       *return_code;   return code of operation        */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  :  function terminates without error              */
/*      D2_ERROR    :  function terminates with error                 */
/*                                                                    */
/*exoff ***************************************************************/
 
 signed32 d21_10_delst(
   signed16      bind_id,
   Errcode      *return_code,
   signed16      version,
   Name_string   subtree_root,
   signed32      serv_cntrls,
   Name_string **not_deleted)
 
{
   signed16    dir_id;
   char        bind_dsa[D2_DNL_MAX + 1];
   D21_p5      pb;                  /* parameter block for SEARCH     */
   D21_p8      pbdel;               /* parameter block for DELETE     */
   char        filename[D27_LFILE_NAME];
   Bool        nocache;
   D2_c_arg    d2_c_arg;
   D2_c_res    c_res;

   struct   d21_bind_info bind_info;
   static   D2_req_info    rinf = {D2_R_VAL_TYP, -1}; /* all attributes.    */	
   static   D2_pbhead      head = {D21V022, D2_NO_ERR, D2_NO_ERR, D2_NO_ERR};
Bool		dummy;
D2_pbhead	dummy_pbhead;
 
d2_c_arg.d2_extension = NULL;
d2_c_arg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
d2_c_arg.d2_op_progress.d2_next_rdn = 0;
d2_c_arg.d2_al_rdn = 0;
d2_c_arg.d2_serv_cntrls = 0;
d2_c_arg.d2_priority = 0;
d2_c_arg.d2_time_limit = 0;
d2_c_arg.d2_size_limit = 0;
d2_c_arg.d2_scope_ref = D2_WORLD_SCOPE;


*not_deleted = NULL;
/*
d21d009_errlog("ENTRY d21_10_delst", (Errcode *)NULL);
*/
 
/*
d27_009_trace_dn("  Subtree:", (char *)subtree_root);
*/

			     /* check version, bind_id */
if (d21t009_param_check(bind_id, return_code, version, &dir_id) == D2_ERROR) 
	{
	/*
      	d21d009_errlog("EXIT  d21_10_delst", return_code);
	*/
      	return(D2_ERROR);
   	}
 
if (d21_bh8_get_bind_info(bind_id, &bind_info, &dummy, &dummy_pbhead, FALSE) 
       != D2_ERROR) 
      	strcpy(bind_dsa, (char *)bind_info.d21_b_name_dsa);
else 
	{
      	return_code->retcode  = CALLERR;
      	return_code->errclass = PARERR;
      	return_code->errvalue = DS_NO_BIND;
	/*
      	d21d009_errlog("EXIT  d21_10_delst", return_code);
	*/
      	return(D2_ERROR);
   	}
 
if ((((serv_cntrls & D2_NOCACHE) == D2_NOCACHE) &&
     ((serv_cntrls & D2_LOCALSCOPE) == D2_LOCALSCOPE)) ||
    (((serv_cntrls & D2_NOCACHE) != D2_NOCACHE) &&
     ((serv_cntrls & D2_LOCALSCOPE) != D2_LOCALSCOPE))) 
	{
      	return_code->retcode  = CALLERR;
      	return_code->errclass = PARERR;
      	return_code->errvalue = DS_SERV_CTRLS_INVALID;
	/*
   	d21d009_errlog("EXIT  d21_10_delst", return_code);
	*/
      	return(D2_ERROR);
   	}
else 
	{
      	sprintf(filename, "%s%d", D21_SEARCH, getpid());
      	nocache = ((serv_cntrls & D2_NOCACHE) == D2_NOCACHE) ? TRUE
								  : FALSE;
      	d2_c_arg.d2_serv_cntrls = serv_cntrls;
      	d2_c_arg.d2_priority    = D2_PRIO_LOW;
      	d2_c_arg.d2_time_limit  = D2_T_UNLIMITED;
      	d2_c_arg.d2_size_limit  = D2_S_UNLIMITED;

      	pb.d21_5pbhead = &head;
      	pb.d21_5c_a = (D2_c_arg *) &d2_c_arg;
      	pb.d21_5bind_id = bind_id; 
      	pb.d21_5base_obj = (subtree_root == NULL) ? (Name_string)NULL :
                         subtree_root;
      	pb.d21_5subset = D2_WHOLE_SUBTREE;
      	pb.d21_5srch_al = FALSE;
      	pb.d21_5f_pattern = NULL;
		/* get all the attributes of the objects.	*/
      	pb.d21_5r_info = &rinf;
      	pb.d21_5file = filename;
	pb.d21_5err = (D2_error *)NULL;
	pb.d21_5cres = &c_res;

                                   /* generate parameter block */
      	pbdel.d21_8pbhead = &head;
      	pbdel.d21_8c_a = (D2_c_arg *) &d2_c_arg;
      	pbdel.d21_8bind_id = bind_id;
      	pbdel.d21_8err = (D2_error *)NULL;
   
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

         	unlink(filename);
		/*
	 	d21d009_errlog("EXIT  d21_10_delst", return_code);
		*/
         	return(D2_ERROR);
      		}
 
      	if (d21t030_rem_objects(filename, return_code, &pb, &pbdel, 
				not_deleted, nocache, bind_dsa) != D2_NOERROR)
		{
      		unlink(filename);
		/*
   		d21d009_errlog("EXIT  d21_10_delst", return_code);
		*/
   		return(D2_ERROR);
		}
   	}

unlink(filename);
/*
d21d009_errlog("EXIT  d21_10_delst", return_code);
*/
return(D2_NOERROR);
}
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE    :   C-FUNCTION                                             */
/*                                                                    */
/* NAME    :   d21t030_rem_objects()                                  */
/*                                                                    */
/* AUTHOR  :   N. Serra, D 525 (CDS)                                  */
/* DATE    :   31.08.89                                               */
/*                                                                    */
/* SYNTAX  :   signed32 d21t030_rem_objects(filename,return_code, pb, */
/*		   pbdel, not_deleted, nocache, bind_dsa)             */
/*								      */
/*             char         *filename;                                */
/*             Errcode      *return_code;                             */
/*             D21_p5       *pb;                                      */
/*             D21_p8       *pbdel;                                   */
/*             Name_string **not_deleted;                             */
/*             Bool          nocache;                                 */
/*             char         *bind_dsa;                                */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*       This function will scan the search result files and it will  */
/*       try to remove all the objects of the subtree and his root.   */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*       char         *filename;       name of the "search" file of   */
/*                                     the subtree                    */
/*       D21_p5       *pb;             parameter block for "search"   */
/*       D21_p8       *pbdel;          parameter block for "delete"   */
/*       Bool          nocache;        test D2_NOCACHE service control*/
/*       char         *bind_dsa;       name of the bind DSA           */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*       Errcode      *return_code;    return code of operation       */
/*    Name_string **not_deleted;    list of objects which were not    */
/*                                     deleted                        */
/*                                                                    */
/* RETURN-VALUE:                                                      */
/*             D2_NOERROR : no errors                                 */
/*             D2_ERROR   : function failed                           */
/*                                                                    */
/*exoff ***************************************************************/
 
 signed32 d21t030_rem_objects(
   char         *filename,
   Errcode      *return_code,
   D21_p5       *pb,
   D21_p8       *pbdel,
   Name_string **not_deleted,
   Bool          nocache,
   char         *bind_dsa)
 
{
   D20_dn_off   dn;                       /* DN-info                */
   D20_dn_off   mk;                       /* master knowledge info  */
   D20_dns      ext_dn;                   /* external DN-list       */
   D20_dns      int_dn;                   /* internal DN-List       */
   int          fd;
   signed32     c1;
   signed32	ret_value = D2_NOERROR;
   signed32	attr_len;

   Bool        *b_notdel = NULL;
 
 
if (d21t032_get_search_file(&fd, filename, return_code, &dn, &mk, &ext_dn, 
			    &int_dn, &attr_len) == D2_ERROR) 
      	return(D2_ERROR);
 
if (dn.no == 0)
	{
	return_code->retcode  = CALLERR;
	return_code->errclass = PARERR;
	return_code->errvalue = DS_NFOUND;
      	d20_111_free((char *) dn.poff, (char *) dn.off, (char *) mk.poff,
		     (char *) mk.off, ext_dn.names, int_dn.names, (char *)NULL,
		     (char *)NULL, (char *)NULL, (char *)NULL);
      	close(fd);
      	return(D2_ERROR);
	}

if ((b_notdel = (Bool *)malloc((size_t)(dn.no) * sizeof(Bool))) == NULL) 
	{
      	return_code->retcode  = PERMERR;
      	return_code->errclass = SYSERR;
      	return_code->errvalue = DS_ALLOC_ERROR;
      	d20_111_free((char *) dn.poff, (char *) dn.off, (char *) mk.poff,
		     (char *) mk.off, ext_dn.names, int_dn.names, (char *)NULL,
		     (char *)NULL, (char *)NULL, (char *)NULL);
      	close(fd);
      	return(D2_ERROR);
   	}

for (c1 = 0; c1 < dn.no; c1++)
      	*(b_notdel + c1) = TRUE;
 
d20_beg1 = d20_beg2 = ext_dn.names;

		      /* sort entry dns                   */
qsort(dn.poff, (size_t) dn.no, (size_t) sizeof(String),
      (int (*)(const void *, const void *)) d20_101_compare_dn);
 
ret_value = d21t031_handle_dns(fd, dn.poff, dn.no, pb, pbdel, 
			       b_notdel + dn.no - 1, nocache, return_code, 
			       bind_dsa, d20_beg1); 
 
if (ret_value != D2_NOERROR) 
	{
      	if (ret_value == D2_ERROR) 
		{
		/* the error is in "remove_object" function */
         	d21t034_not_deleted(dn.no, dn.poff, not_deleted, b_notdel);
         	return_code->errvalue = DS_DEL_ERR;
      		}

      	ret_value = D2_ERROR;
      	return_code->retcode  = PERMERR;
      	return_code->errclass = SYSERR;
   	}
 
d20_111_free((char *) dn.poff, (char *) dn.off, (char *) mk.poff,
	     (char *) mk.off, ext_dn.names, int_dn.names, (char *)NULL,
	     (char *)NULL, (char *)NULL,(char *)NULL);

d20_111_free((char *) b_notdel, (char *)NULL, (char *)NULL, 
             (char *)NULL, (char *)NULL, (char *)NULL,
             (char *)NULL, (char *)NULL, (char *)NULL,
             (char *)NULL);

close(fd);
return(ret_value);

}
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t031_handle_dns()                                  */
/*                                                                    */
/* AUTHOR:      N. Serra, D 525 (CDS)                                 */
/* DATE:        05.09.89                                              */
/*                                                                    */
/* SYNTAX:      signed32 d21t031_handle_dns(fd, dn, dn_no, pb, pbdel, */
/*                          b_notdel, nocache, return_code,           */
/*                          bind_dsa, d20_beg)                        */
/*                                                                    */
/*		int	     fd;				      */
/*              D20_offset **dn;                                      */
/*              signed32     dn_no;                                   */
/*              D21_p5      *pb;                                      */
/*              D21_p8      *pbdel;                                   */
/*              Bool        *b_notdel;                                */
/*              Bool         nocache;                                 */
/*              Errcode     *return_code;                             */
/*              char        *bind_dsa;                                */
/*              char        *d20_beg;                                 */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will try to remove the objects that     */
/*              are in the "dn" list.                                 */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*		int	     fd;	search file descriptor.       */
/*              D20_offset **dn;        pointer list to DNs in DSA1   */
/*              signed32     dn_no;     number of DNs in DSA1         */
/*              D21_p5      *pb;        parameter block for "search"  */
/*              D21_p8      *pbdel;     parameter block for "delete"  */
/*              Bool         nocache;   test NOCACHE service control  */
/*              char        *bind_dsa;  name of the bind DSA          */
/*              char        *d20_beg;   start address of "dn" strings */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              Bool        *b_notdel;  objects not deleted           */
/*              Errcode     *return_code;  return code of operation   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              D2_NOERROR: operation is ok                           */
/*              D2_ERROR:   remove object error 		      */
/*		NO_REM_ERROR	not a remove object error	      */
/*				but some other error.		      */
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
 
 signed32 d21t031_handle_dns(
   int          fd,
   D20_offset **dn,
   signed32     dn_no,
   D21_p5      *pb,
   D21_p8      *pbdel,
   Bool        *b_notdel,
   Bool         nocache,
   Errcode     *return_code,
   char        *bind_dsa,
   char        *d20_beg)
 
{                                     /*  d21t031_handle_dns                */
   D20_offset **dn_max = dn + dn_no;
   D20_offset **dn_sav = dn;
   D2_a_info   *attr_ptr;
   char        *mk_attr = NULL;
   char         norm_mk_attr[D2_DNL_MAX + 1];   
   char         norm_bind_dsa[D2_DNL_MAX + 1];   
   signed16     mk_len;
   signed32     min_pos = 0;
   signed32     max_pos = 0;
   signed32	ret_value = D2_NOERROR;
   char        *at_mem;
   char         par_obj[D2_DNL_MAX + 1];
   Bool         deleteobj;
   D2_a_type    MK; 
 
   static   D2_a_type      reqinfo1 = {sizeof(D2_MASTER_KNOWLEDGE)-1,
				       (Ob_id_string) D2_MASTER_KNOWLEDGE};
   static   D2_req_info    rinf1p = {D2_R_VAL_TYP, 1, &reqinfo1};
 
MK.d2_typ_len = sizeof(D2_MASTER_KNOWLEDGE) - 1;
MK.d2_type = (Ob_id_string)D2_MASTER_KNOWLEDGE;

/* norm name for bind dsa */
strcpy(norm_bind_dsa, bind_dsa);
d20_norm(norm_bind_dsa, strlen(norm_bind_dsa));

dn = dn_max - 1;
if (nocache == TRUE) 
	{	/* delete only master entries         */
      	while (dn >= dn_sav) 
		{	/* exist objects in the DN-list       */
		     	/* delete master                      */
	 	pbdel->d21_8entry = (Name_string)d20_beg + (*dn)->d20_mem_off;
         	if (d21_8_remove_object(pbdel) != D2_NOERROR) 
			{
            		b_notdel--;
            		ret_value = D2_ERROR;
         		}
	 	else 
			/* put it in the "not_deleted" list   */
            		*b_notdel-- = FALSE;   

	 	/* take next DN from the list         */
		dn--;
      		}       
   	}
else 
	{
		/* case of bind DSA or specific DSA.	*/
		/* get memory for the attributes      */
      	if ((at_mem = malloc(D20_BUFSIZ)) == NULL) 
		{
         	return_code->errvalue = DS_ALLOC_ERROR;
         	ret_value = NO_REM_ERROR;
         	return(ret_value);
      		}

	while (dn >= dn_sav) 
		{         /* exist objects in the DN-list       */
		/* the object is not an alias         */
		if (((*dn)->d20_state & D21_ALIAS_OBJ) == 0) 
			{
	     		/* read his Master Knowledge          */
       			min_pos = max_pos = 0;
               		if (d21t008_get_attr(fd, *dn, at_mem, &min_pos, 
				&max_pos, MK, &mk_attr, &mk_len, d20_beg) 
								== D2_ERROR)
				{
				/* MK attr. not found    */
            			d20_111_free(at_mem, (char *)NULL, 
				(char *)NULL, (char *)NULL, 
				(char *)NULL, (char *)NULL,
				(char *)NULL, (char *)NULL, 
				(char *)NULL, (char *)NULL);
            			return_code->errvalue = DS_SEARCH_ERR;
            			return(NO_REM_ERROR);
				}
			/* norm name for master-knowledge attr. */
			strcpy(norm_mk_attr, mk_attr);
			d20_norm(norm_mk_attr, strlen(norm_mk_attr));
         		}
 
         	if ((((*dn)->d20_state & D21_ALIAS_OBJ) == 0) && 
					(!strcmp(norm_mk_attr, norm_bind_dsa))) 
			{ 
		     	/* the object is not an alias and it is a master entry*/
		     	/* set service controls according to  master entries  */

            		deleteobj = TRUE;
            		(pbdel->d21_8c_a)->d2_serv_cntrls |= D2_NOCACHE;
            		(pbdel->d21_8c_a)->d2_serv_cntrls &= ~D2_LOCALSCOPE;
            		d20_172_set_master_serv_cntrls();
         		}
	 	else 
			{                                
			/* "dn" is a shadow entry   */
			/* or it is an alias object.	*/
			/* if it is an alias delete the object in any case  */
			/* and set service controls depending on the MK of  */
			/* the parent.					    */
			/* if it is a shadow delete only if parent is also  */
			/* shadow.					    */
			/* search the parent of the object in memory and */
			/* look if it is a master or a shadow entry      */

            		if (d21t033_parent_object_name(d20_beg + 
					(*dn)->d20_mem_off, par_obj) == TRUE)
               			pb->d21_5base_obj = (D2_name_string)par_obj;
            		else 
		    		/* the object is under the root: 	*/
				/* read the MK of Scheme 		*/
               			pb->d21_5base_obj = 
						(D2_name_string)D2_NAME_SCHEME;

            		pb->d21_5subset = D2_BASE_OBJECT;
            		pb->d21_5r_info = &rinf1p;
				/* search in memory         */
            		pb->d21_5file = NULL;            
            		pb->d21_5c_a->d2_serv_cntrls &= ~D2_NOCACHE;
            		pb->d21_5c_a->d2_serv_cntrls |= D2_LOCALSCOPE;
            		d20_168_set_serv_cntrls();

            		if (d21_5_search(pb) == D2_NOERROR) 
				{
               			attr_ptr = pb->d21_5res->d2_ei_ai;
               			while (attr_ptr != NULL &&
		  			D2_ATTCMP(&(attr_ptr->d2_a_type),
					D2_MASTER_KNOWLEDGE)!=0)
                  			attr_ptr = attr_ptr->d2_a_next; 

               			if (attr_ptr == NULL)
					/* "search" error           */
                  			ret_value = NO_REM_ERROR;     
               			else
                  			mk_attr =
	       					(char *)pb->d21_5res->d2_ei_ai->
						d2_a_val->d2_at_value.
						d2_obj_name_a_v;
					/* norm name for master-knowledge attr*/
					strcpy(norm_mk_attr, mk_attr);
					d20_norm(norm_mk_attr,
						 strlen(norm_mk_attr));
               			}
            		else
				/* "search" error           */
	       			ret_value = NO_REM_ERROR;       

 
            		if (ret_value == NO_REM_ERROR) 
				{
    				if (pb->d21_5res != (D2_ei_info *) NULL)
    					free((char *) pb->d21_5res);

               			d20_111_free(at_mem, (char *)NULL, 
				(char *)NULL, (char *)NULL, 
				(char *)NULL, (char *)NULL,
				(char *)NULL, (char *)NULL, 
				(char *)NULL, (char *)NULL);
               			return_code->errvalue = DS_SEARCH_ERR;
               			return(ret_value);
            			}

			/* it is an alias object.			     */
			/* delete this object whether parent is master or    */
			/* shadow.					     */
         		if (((*dn)->d20_state & D21_ALIAS_OBJ) != 0)
				{
				deleteobj = TRUE;
				if (strcmp(norm_mk_attr, norm_bind_dsa) == 0)
					{
					/* it is a master entry.	*/
                      			/* set service controls for 	      */
					/* master entries  		      */
            				(pbdel->d21_8c_a)->d2_serv_cntrls |= 
								D2_NOCACHE;
            				(pbdel->d21_8c_a)->d2_serv_cntrls &= 
								~D2_LOCALSCOPE;
            				d20_172_set_master_serv_cntrls();
    					free((char *) pb->d21_5res);
					}
				else
					{
                      			/* set service controls for 	      */
					/* shadow entries  		      */
            				(pbdel->d21_8c_a)->d2_serv_cntrls &= 
								~D2_NOCACHE;
            				(pbdel->d21_8c_a)->d2_serv_cntrls |= 
								D2_LOCALSCOPE;
            				d20_168_set_serv_cntrls();
    					free((char *) pb->d21_5res);
					}
				}
			else
				{
		    		/* delete the object only if his parent is a */
				/* shadow   				     */
            			deleteobj = (strcmp(norm_mk_attr,
						norm_bind_dsa)) ? TRUE : FALSE;

                      		/* set according service controls for shadow */
				/* entries  				     */
            			(pbdel->d21_8c_a)->d2_serv_cntrls &= 
								~D2_NOCACHE;
            			(pbdel->d21_8c_a)->d2_serv_cntrls |= 
								D2_LOCALSCOPE;
            			d20_168_set_serv_cntrls();
    				free((char *) pb->d21_5res);
				}
         		}

         	if (deleteobj == TRUE) 
			{
		       /* delete object            */
	    		pbdel->d21_8entry = 
			(Name_string)d20_beg + (*dn)->d20_mem_off;

            		if (d21_8_remove_object(pbdel) != D2_NOERROR) 
				{
               			b_notdel--;
               			ret_value = D2_ERROR;
            			}
	    		else 
				/* put it in the "not_deleted" list   */
               			*b_notdel-- = FALSE;
         		}
         	else 
			{
            		b_notdel--;
            		ret_value = D2_ERROR;
         		}

               	/* take next DN from the list         */
         	dn--;         
      		}       

      	d20_168_set_serv_cntrls(); 
      	d20_111_free(at_mem, (char *)NULL, (char *)NULL, 
                   (char *)NULL, (char *)NULL, (char *)NULL,
                   (char *)NULL, (char *)NULL, (char *)NULL,
                   (char *)NULL);
   	}

return(ret_value);

}                                     /*  d21t031_handle_dns                */
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t032_get_search_file()                             */
/*                                                                    */
/* AUTHOR:      N. Serra, D 525 (CDS)                                 */
/* DATE:        27.09.89                                              */
/*                                                                    */
/* SYNTAX:      signed32 d21t032_get_search_file(fd, filename,        */
/*			return_code, dn, mk, ext_dn, int_dn, attr_len)*/
/*                                                                    */
/*              int         *fd;                                      */
/*              char        *filename;                                */
/*              Errcode     *return_code;                             */ 
/*              D20_dn_off  *dn;                                      */
/*              D20_dn_off  *mk;                                      */ 
/*              D20_dns     *ext_dn;                                  */ 
/*              D20_dns     *int_dn;                                  */ 
/*		signed32    *attr_len;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function get the offsets of the search file.     */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              char        *filename;     name of the search file    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              int         *fd;           file descriptor            */
/*              Errcode     *return_code;  return code                */ 
/*              D20_dn_off  *dn;           DN info                    */
/*              D20_dn_off  *mk;           master knowledge info      */ 
/*              D20_dns     *ext_dn;       external DN-list           */ 
/*              D20_dns     *int_dn;       internal DN-list           */ 
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              D2_NOERROR  :  operation is ok                        */
/*              D2_ERROR    :  operation fails                        */
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
 
 signed32 d21t032_get_search_file(
   int        *fd,
   char       *filename,
   Errcode    *return_code,
   D20_dn_off *dn,
   D20_dn_off *mk,
   D20_dns    *ext_dn,
   D20_dns    *int_dn,
   signed32   *attr_len)
 
{
   signed32     no_bytes = 0;               /* actual number of bytes */
                                            /* read in search file    */
 
   dn->poff = mk->poff = NULL;
   dn->off = mk->off = NULL;
   int_dn->names = ext_dn->names = NULL;
 
   int_dn->max_anz = D20_MORE;
   ext_dn->max_anz = D20_DN_SIZE;
   int_dn->anz = ext_dn->anz = 0;
 
   dn->no = mk->no = 0;
   dn->max_no = D20_DN_NO;
   mk->max_no = D20_INT_DN_NO;
   *fd = -1;
 
if (filename[0] != D2_EOS) 
	{
      	if ((*fd = open(filename, O_RDONLY)) == -1) 
		{
         	return_code->retcode  = PERMERR;
         	return_code->errclass = SYSERR;
         	return_code->errvalue = DS_DEL_ERR;
         	return(D2_ERROR);
      		} 
	else 
		{
		/* no memory for offsets              */
         	if 
			((dn->poff = (D20_offset **)malloc((size_t)dn->max_no
					    * sizeof(String))) == NULL ||
          		 (dn->off = (D20_offset *)malloc((size_t) dn->max_no *
                                         sizeof(D20_offset))) == NULL ||
          		 (mk->poff = (D20_offset **)malloc((size_t)mk->max_no
					    * sizeof(String))) == NULL ||
          		 (mk->off = (D20_offset *)malloc((size_t) mk->max_no *
                                         sizeof(D20_offset))) == NULL ||
          		 (ext_dn->names = malloc((size_t)ext_dn->max_anz)) ==
 								 NULL ||
          		 (int_dn->names = malloc((size_t)int_dn->max_anz)) ==
								 NULL) 
			{
         		return_code->retcode  = PERMERR;
         		return_code->errclass = SYSERR;
         		return_code->errvalue = DS_ALLOC_ERROR;
         		d20_111_free((char *) dn->poff, (char *) dn->off,
				     (char *) mk->poff, (char *) mk->off, 
				     ext_dn->names, int_dn->names,
				     (char *)NULL,(char *)NULL,
				     (char *)NULL,(char *)NULL);
         		close(*fd);
         		return(D2_ERROR);
      			}
     		}

      	*attr_len = 0L;
 
      	if (d20_106_get_dn_offsets(FALSE, *fd, ext_dn, int_dn, dn, mk,
                                 &no_bytes, attr_len)) 
		{
         	return_code->retcode  = PERMERR;
         	return_code->errclass = SYSERR;
         	return_code->errvalue = DS_DEL_ERR;
         	d20_111_free((char *) dn->poff, (char *) dn->off,
			     (char *) mk->poff, (char *) mk->off,
			     ext_dn->names, int_dn->names,
			     (char *)NULL,(char *)NULL,
			     (char *)NULL,(char *)NULL);
         	close(*fd);
         	return(D2_ERROR);
      		}
   	}

return(D2_NOERROR); 

}
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t033_parent_object_name()                          */
/*                                                                    */
/* AUTHOR:      N. Serra, D 525 (CDS)                                 */
/* DATE:        27.09.89                                              */
/*                                                                    */
/* SYNTAX:      Bool d21t033_parent_object_name(obj, par_obj)         */
/*                                                                    */
/*              char    *obj;                                         */
/*              char    *par_obj;                                     */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function looks for the parent object name of     */
/*              an object.                                            */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              char    *obj;       name of the object                */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              char    *par_obj;   name of the parent object         */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              TRUE   :  the parent object exists                    */
/*              FALSE  :  the parent object is the object "root"      */
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
 
 Bool d21t033_parent_object_name(
   char *obj,
   char *par_obj)
 
{
   char  *end_obj;
   signed32 len_par;
 
   end_obj = obj + strlen(obj);

   for (; end_obj != obj; end_obj--) 
      if (*end_obj == D2_RDN_SEP)
          break; 
   

   if (end_obj == obj)
      return(FALSE);

   len_par = strlen(obj) - strlen(end_obj);
   strncpy(par_obj, obj, len_par);
   par_obj[len_par] = D2_EOS;
   return(TRUE);
}
 
/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21t034_not_deleted()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function creates a list of not deleted objects.          */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed32       dn_no;       number of DNs handled so far      */
/*      D20_offset   **pdn;         pointer array refrencing DNs      */
/*                                  (DNs not leading to name          */
/*                                  collisions are marked with D2_NULL*/
/*      Bool          *dn_name_col; name collision for every DN       */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Name_string  **not_deleted; pointer of name collisions        */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  = The function has been performed successfully.   */
/*      D2_ERROR    = function fails                                  */
/*                                                                    */
/*exoff ***************************************************************/
 
 signed32 d21t034_not_deleted(
   signed32      dn_no,
   D20_offset  **pdn,
   Name_string **not_deleted,
   Bool         *dn_name_col)
 
{
   signed32      i;
   signed32      length = 0;          /* total length of all DN strings     */
   signed32      no = 0;              /* total number of all DNs            */
   signed32      mem_length;
   char         *p_dn;
   char         *dn_area;
   Name_string  *ptr_area;
   D20_offset  **poff;
   Bool         *sav_dn_col = dn_name_col;
 
   for (i = 0, poff = pdn; i < dn_no; i++, poff++, sav_dn_col++) {
      if (*sav_dn_col == TRUE) {
         no++;
         length += strlen(d20_beg1 + (*poff)->d20_mem_off) + 1;
      }
   }
 
   if (no != 0) {
      mem_length = D2_ALIGN((no + 1) * sizeof(String)) + D2_ALIGN(length);
      if ((*not_deleted = (Name_string *) malloc((size_t) mem_length))
                          == NULL) {
         return(D2_ERROR);
      }
      else {
         ptr_area = *not_deleted;
	 dn_area = (char *) ptr_area + D2_ALIGN((no + 1) *  sizeof(String));
 
         for (i = 0; i < dn_no; i++, pdn++, dn_name_col++) {
            if (*dn_name_col == TRUE) {
               *ptr_area = (Name_string)dn_area;
               p_dn = d20_beg1 + (*pdn)->d20_mem_off;
               strcpy(dn_area, p_dn);
               dn_area += strlen(p_dn) + 1;
               ptr_area++;
            }
         }
 
         *ptr_area = NULL;
      }
   }
   return(D2_NOERROR);
}
