/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21chnam.c,v $
 * Revision 1.1.718.2  1996/02/18  19:39:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:56  marty]
 *
 * Revision 1.1.718.1  1995/12/08  15:15:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:17  root]
 * 
 * Revision 1.1.716.5  1994/06/21  14:43:23  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:09:19  marrek]
 * 
 * Revision 1.1.716.4  1994/05/20  12:57:01  marrek
 * 	Fix for subt.[12]chname testcases. (OT10670)
 * 	[1994/05/20  12:09:32  marrek]
 * 
 * Revision 1.1.716.3  1994/05/10  15:49:17  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:09:34  marrek]
 * 
 * Revision 1.1.716.2  1994/03/23  15:04:52  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:07:09  keutel]
 * 
 * Revision 1.1.716.1  1994/02/22  15:57:57  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:24:24  marrek]
 * 
 * Revision 1.1.714.2  1993/10/14  16:46:17  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:42:46  keutel]
 * 
 * Revision 1.1.714.1  1993/10/13  17:29:22  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  07:13:36  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21chnam.c,v $ $Revision: 1.1.718.2 $ $Date: 1996/02/18 19:39:54 $";
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
/* NAME         : d21chnam.o                                          */
/*                                                                    */
/* AUTHOR       : Serra & Pulpillo, D525 (CDS)                        */
/* DATE         : 24.10.89                                            */
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
/*      0.1 | 20.09.89| Erstellung                     | hv |         */
/*                                                                    */
/*datoff **************************************************************/
 
 
/* Includes */

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
 
#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d21iapl.h>
#include <d21.h>
#include <d2info.h>
#include <d2delupd.h>
#include <d2dump.h>
#include <d2adm.h>
#include <d20proto.h>
 

/* Defines */
 
#define D21_SAVE        "CHNAME"/* Name of the saved file             */
#define D21_SEARCH      "SEARCH"/* Name of the saved file             */

#define NOT_REAL         0      /* Working in local scope mode        */
#define REAL             1      /* Working in nocache mode            */

#define CONVERR(value) (value % 16 + '0') /* Conversion short -> char */
 
/* Variables */
 
  D2_pbhead  *d20_head; 

extern  char       *d20_beg1;           /* memory address of DNs      */
extern  char       *d20_beg2;           /* memory address of DNs      */

static   struct  d21_bind_info bind_info;
 
extern  Ds_v2_info  info;
extern  signed16    d20_bid;            /* global bind Identifier     */

static    D2_credent *d21_usercred;
 
/* Functions */

static void d21t041_get_rdn(char *obj, char *rdn);
static signed32 d21t043_compare_rdn(D20_rdn_tbl **rdn1, D20_rdn_tbl **rdn2);

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21_15_chnam()                                             */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will change the distinguished name of the       */
/*      master entry of a directory object from an old to a new       */
/*      value. The old distinguished name must not necessarily        */
/*      correspond to a "leaf entry", it can also correspond to an    */
/*      intermediate node in the directory tree.                      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed16      bind_id;          session identification        */
/*      signed16      version;          version of interface          */
/*      Name_string   old_dname;        old distinguished name of the */
/*                                      object                        */
/*      Name_string   new_dname;        new distinguished name of the */
/*                                      object                        */
/*      Bool          overwrite;        existing objects have to be   */
/*                                      overwrited or not             */
/*      Name_string **conflicts;        list of objects with          */
/*                                      conflicting names             */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode     *return_code;       return code of operation      */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      NOERROR     = The function has been performed successfully.   */
/*      ERROR       = function fails                                  */
/*                                                                    */
/*exoff ***************************************************************/
 
 signed32 d21_15_chnam(
   signed16      bind_id,
   Errcode      *return_code,
   signed16      version,
   Name_string   old_dname,
   Name_string   new_dname,
   Bool          overwrite,
   Name_string **conflicts)
 
{
   char           *d20_beg;            /* start address of DN-strings*/
   signed32       ret_value = D2_NOERROR;
   D2_c_arg       c_arg;
   char           file[D27_LFILE_NAME]; /* Name of the search file    */
   char           file2[D27_LFILE_NAME];/* Name of the save file      */
   char           file3[D27_LFILE_NAME];/* Name of the save file      */
 
   D20_pB         pb;
   D2_pbhead      head;
   D20_delupd_info del_info;            /* Information to be used for */
   signed32       val[D20_NOVAL];       /* delta updates              */
 
   char           bind_dsa[D2_DNL_MAX+1]; /* Name of the bind DSA     */
   char           curr_dsa[D2_DNL_MAX+1]; /* Name of the current DSA  */
   char           norm_curr_dsa[D2_DNL_MAX+1];
					 /* Name of the current DSA (normed) */
   signed16       curr_id;
   char           old_dn[D2_DNL_MAX+1];   /* Old DN normalized        */
   char           new_dn[D2_DNL_MAX+1];   /* New DN normalized        */
 
   int            fd,fd2;
   D20_offset    *dn,*dn2;      /* Offset of DN relatives to begining */
                                /* of file or memory areas            */
   D20_offset    **pdn,**pdn2;
   signed32      dn_no,dn_no2;  /* Number of DNs found                */
   char         *names,*name2;  /* Pointer to memory area for DNs     */
   signed32      l_name2;/* Length of DN list                  */
   signed32      attr_len,attr2_len;/* Lenght of attributes           */
 
   signed32       max_pos,min_pos;
   signed32       max_pos2,min_pos2;
 
   char          *mk_attr=NULL;      /* Master Knowledge attribute */
   char           norm_mk_attr[D2_DNL_MAX+1];
				     /* Master Knowledge attribute (normed) */
   signed16       mk_len;

   char          *at_mem,*at_mem2;
 
   D20_dsa_tbl    num_dsa[D20_INT_DN_NO];/* DSAs                      */
   D20_rdn_tbl   *num_rdn, *first_num_rdn;/* Relatives Disting. names */
   D20_rdn_tbl  **prdn,**first_prdn;    /* pointer list to num_rdn    */
 
   char           object[D2_DNL_MAX+1];
   char           auxobj[D2_DNL_MAX+1];
   char           par_auxobj[D2_DNL_MAX+1];
   char           rdn[D2_NP_LEN+1];
   D20_rdn_tbl    par_name, *par_n[1], **p_num_rdn;
   char         **area_aux, **first_area_aux;
   char          *area_rdn, *first_area_rdn;
   D20_offset   **ptr, **found;
   char           full_name[D2_DNL_MAX+1];
   char           n_name[D2_DNL_MAX+1];
   signed16       index;
   unsigned32     i, n_elem;
   signed16       dir_id;
   Bool           end_loop;
   char         **not_del, **ptr_del;
   Bool          *b_conflicts = FALSE;
   signed16       save_bid;
   char           ovr[15];
   D2_a_type      MK; 
   D2_a_type      AL;
   char		  *at_val_area;
Bool		dummy;
D2_pbhead	dummy_pbhead; 



/*
d21d009_errlog("ENTRY d21_15_chnam", NULL);
*/
/*
d27_009_trace_dn("  Old Object:", (char *)old_dname);
*/
/*
d27_009_trace_dn("  New Object:", (char *)new_dname);
*/
sprintf(ovr, "%d", overwrite);
/*
d27_009_trace_dn("  Overwrite:", ovr);
*/
 
MK.d2_typ_len = sizeof(D2_MASTER_KNOWLEDGE) - 1;
MK.d2_type = (Ob_id_string)D2_MASTER_KNOWLEDGE;

AL.d2_typ_len = sizeof(D2_ALIASED_OBJECT) - 1;
AL.d2_type = (Ob_id_string)D2_ALIASED_OBJECT;

/* Initializing pointers               */
dn = dn2 = (D20_offset *)NULL;
pdn = pdn2 = (D20_offset **)NULL;
names = name2 = NULL;

/* check version, bind-id */
if (d21t009_param_check(bind_id, return_code, version, &dir_id) == D2_ERROR)
	{
	/*
      	d21d009_errlog("EXIT  d21_15_chnam", return_code);
	*/
      	return(D2_ERROR);
   	}

strcpy(old_dn, (char *)old_dname);        /* Old distinguished name        */
d20_norm(old_dn, strlen(old_dn)); /* normalized                    */
strcpy(new_dn, (char *)new_dname);        /* New distinguished name        */
d20_norm(new_dn, strlen(new_dn)); /* normalized                    */

/* old_name is same as new_name.	*/
if (!strcmp((char *)old_dn, (char *)new_dn)) 
	{
	/*
      	d21d009_errlog("EXIT  d21_13_chnam", return_code);
	*/
      	return(NOERROR);
   	}

/* Take name of the bind DSA           */
if (d21_bh8_get_bind_info(bind_id, &bind_info, &dummy,&dummy_pbhead,FALSE) 
       != D2_ERROR) 
	{
      	d21_usercred = bind_info.d21_b_credent ;
      	strcpy(bind_dsa, (char *)bind_info.d21_b_name_dsa);
      	strcpy(curr_dsa,bind_dsa);    /* DSA currently binded           */
      	curr_id=bind_id;
   	}
else 
	{
      	return_code->retcode  = CALLERR;
      	return_code->errclass = PARERR;
      	return_code->errvalue = DS_NO_BIND;
	/*
      	d21d009_errlog("EXIT  d21_15_chnam", return_code);
	*/
      	return(D2_ERROR);
   	}

*conflicts = NULL;
 
   /*******************************************************************/
   /*                 Look for the New distinguished name subtree     */
   /*		      	(master entries are searched.)		      */
   /*******************************************************************/
 
d20_head = pb.d20_Bpbhead = &head;
head.d2_version = DS_V02;
head.d2_retcode = head.d2_errclass = head.d2_errvalue = D2_NO_ERR;
 
save_bid = d20_bid;
d20_bid = pb.d20_B2bind_id = bind_id;
pb.d20_B2c_a = &c_arg;
c_arg.d2_serv_cntrls = D2_NOREFERRAL | D2_USEDSA | D2_NOCACHE | D2_DONT_STORE |
		       D2_DREFALIAS | D2_PREF_ADM_FUN;
c_arg.d2_priority = D2_PRIO_LOW;
c_arg.d2_time_limit = D2_T_UNLIMITED;
c_arg.d2_size_limit = D2_S_UNLIMITED;

sprintf(file2, "%s%d", D20_SE2, getpid());

d20_172_set_master_serv_cntrls();

/* Search of the new DN and its subordinates.	*/
if (d20_165_call_search(pb.d20_B2bind_id, FALSE, pb.d20_B2c_a,
                           (D2_name_string) new_dn, file2, FALSE, &fd2, &name2,
                           &l_name2, (D20_offset **) &pdn2, &dn2, &dn_no2,
			   &attr2_len) != D2_NOERROR) 
	{
         return_code->retcode  = CONVERR(pb.d20_Bpbhead->d2_retcode);
         return_code->errclass = CONVERR(pb.d20_Bpbhead->d2_errclass);
         return_code->errvalue = pb.d20_Bpbhead->d2_errvalue;

         if (pb.d20_Bpbhead->d2_errvalue == DS_NFOUND) 
            	return_code->errvalue = DS_PAR_NFOUND;

      	 unlink(file2);
         d20_bid = save_bid;
	 /*
	 d21d009_errlog("EXIT  d21_15_chnam", return_code);
	 */
         return(D2_ERROR);
   	}
 
   /*******************************************************************/
   /* 	save old DN subtree.					      */
   /*   (master entries are searched.)				      */
   /*******************************************************************/
 
sprintf(file,"%s%d",D21_SAVE,getpid());
sprintf(file3,"%s%d_1",D21_SAVE,getpid());

c_arg.d2_serv_cntrls = D2_NOCACHE | D2_PREF_ADM_FUN;

if (d21_14_save(bind_id, return_code, version, (Name_string) old_dn,
      		OBJECT_SUBORDINATES, c_arg.d2_serv_cntrls, file)==D2_ERROR) 
	{
      	close(fd2);
      	unlink(file2);
      	unlink(file);
      	unlink(file3);

      	d20_bid = save_bid;
	/*
      	d21d009_errlog("EXIT  d21_15_chnam", return_code);
	*/
      	return(D2_ERROR);
   	}

d20_head = &head;
 
if (d20_120_read_delupd_info(file, val, &del_info) == D2_ERROR) 
	{
      	return_code->retcode = CONVERR(d20_head->d2_retcode);
      	return_code->errclass = CONVERR(d20_head->d2_errclass);
      	return_code->errvalue = d20_head->d2_errvalue;

        close(fd2);
        unlink(file2);
      	unlink(file);
      	unlink(file3);

      	d20_bid = save_bid;
	/*
      	d21d009_errlog("EXIT  d21_15_chnam", return_code);
	*/
      	return(D2_ERROR);
   	}

c_arg.d2_serv_cntrls = D2_NOREFERRAL | D2_USEDSA | D2_LOCALSCOPE | 
		       D2_DONT_STORE | D2_DREFALIAS | D2_PREF_ADM_FUN;

d20_init();

/*  read dump file                 */
if (d20_116_read_dump_file(file, &names, (D20_offset **) &pdn, &dn,
			   &dn_no,&attr_len, &del_info)) 
	{
       	/*  operation fails                */
      	return_code->retcode = PERMERR;
      	return_code->errclass = SYSERR;
      	return_code->errvalue = pb.d20_Bpbhead->d2_errvalue;

        close(fd2);
        unlink(file2);
      	unlink(file);
      	unlink(file3);

      	d20_bid = save_bid;
	/*
      	d21d009_errlog("EXIT  d21_15_chnam", return_code);
	*/
      	return(D2_ERROR);
   	}

/*  open SEARCH-file of old DN subtree	*/
if ((fd = open(file, O_RDONLY)) == -1) 
	{
       	/*  open fails                     */
      	if (errno == ENOENT || errno == EACCES) 
	 	return_code->errvalue = DS_NO_FILE;
      	else 
         	return_code->errvalue = DS_FILE_ERR;

      	return_code->retcode = PERMERR;
      	return_code->errclass = SYSERR;

        close(fd2);
        unlink(file2);
      	unlink(file);
      	unlink(file3);

      	d20_bid = save_bid;
	/*
      	d21d009_errlog("EXIT  d21_15_chnam", return_code);
	*/
      	return(D2_ERROR);
   	}

/* Allocate memory for the RDN table and attributes */
if (d21t044_get_memory(dn_no + dn_no2, &num_rdn, (D20_rdn_tbl **) &prdn,
	attr_len, &at_mem, attr2_len, &at_mem2, (char **) &first_area_aux) 
			== D2_ERROR) 
	{
      	return_code->retcode  = PERMERR;
      	return_code->errclass = SYSERR;
      	return_code->errvalue = DS_ALLOC_ERROR;
      	d20_111_free((char*) dn, (char*) pdn, names, (char*) num_rdn,
		(char*) prdn, (char*) dn2, (char *) pdn2, name2, NULL, NULL);

        close(fd2);
        unlink(file2);
      	close(fd);
      	unlink(file);
      	unlink(file3);

      	d20_bid = save_bid;
	/*
      	d21d009_errlog("EXIT  d21_15_chnam", return_code);
	*/
      	return(D2_ERROR);
   	}

first_area_rdn=(char *)first_area_aux + (dn_no*sizeof(char *));
 
if ((at_val_area = (char *)malloc(attr_len + attr2_len + 
			 (D2_AT_MAX * max_comp_attr() * 2))) == (char *) NULL)
	{
      	return_code->retcode  = PERMERR;
      	return_code->errclass = SYSERR;
      	return_code->errvalue = DS_ALLOC_ERROR;
      	d20_111_free((char *) dn, (char *) pdn, names, (char *) num_rdn,
		(char *) prdn, (char *) dn2, (char *) pdn2, name2, NULL, NULL);

        close(fd2);
        unlink(file2);
      	close(fd);
      	unlink(file);
      	unlink(file3);

      	d20_bid = save_bid;
	/*
      	d21d009_errlog("EXIT  d21_15_chnam", return_code);
	*/
      	return(D2_ERROR);
	}

if ((b_conflicts = (Bool *)malloc((dn_no) * sizeof(Bool)))
       == (Bool *) NULL) 
	{
      	return_code->retcode  = PERMERR;
      	return_code->errclass = SYSERR;
      	return_code->errvalue = DS_ALLOC_ERROR;
      	d20_111_free((char *) dn, (char *) pdn, names, (char *) num_rdn,
		(char *) prdn, (char *) dn2, (char *) pdn2, name2, at_val_area,
		NULL);

        close(fd2);
        unlink(file2);
      	close(fd);
      	unlink(file);
      	unlink(file3);

      	d20_bid = save_bid;
	/*
      	d21d009_errlog("EXIT  d21_15_chnam", return_code);
	*/
      	return(D2_ERROR);
   	}

for (i = 0; i < dn_no; i++) 
      	*(b_conflicts + i) = TRUE;

   /*******************************************************************/
   /*       Adding RDN of the new parent objects into the table       */
   /*******************************************************************/
 
first_prdn=prdn;
first_num_rdn=num_rdn;
d20_beg=name2;                   /* Initial address of the DN's    */
ptr=(D20_offset **)pdn2;

min_pos = max_pos = 0;
min_pos2 = max_pos2 = 0;

for(i=0; i < dn_no2; i++,ptr++) 
	{
      	strcpy(object, d20_beg + (*ptr)->d20_mem_off);

	/* read the Master Knowledge                      */
       	/* If the object belongs to the subtree */

      	if (d21t042_filter_obj(object,new_dn) != D2_EXTERN_OBJ) 
		{
		if (((*ptr)->d20_state & D21_ALIAS_OBJ) == 0) 
			{
			/* object is not an alias object.	*/
         		if (d21t008_get_attr(fd2, *ptr, at_mem2, &min_pos2, 
				&max_pos2, MK, &mk_attr, &mk_len, d20_beg) ==
								D2_ERROR)
				{
            			return_code->retcode = 
						CONVERR(d20_head->d2_retcode);
            			return_code->errclass = 
						CONVERR(d20_head->d2_errclass);
            			return_code->errvalue = d20_head->d2_errvalue;
            			ret_value=D2_ERROR;
            			break;
         			}
			}
		else
			{
			/* object is an alias.	*/
			/* get the master knowledge of the parent object.    */

			/* if object is not directly under root, parent must */
			/* exist in the search result.			     */

            		if (d21t033_parent_object_name(object, auxobj) != FALSE)
				{
         			found=d21t040_search_dn(pdn2, dn_no2, name2, 
						(Name_string) auxobj);
         			if (d21t008_get_attr(fd2, *found, at_mem2, 
					&min_pos2, &max_pos2, MK, &mk_attr, 
					&mk_len, d20_beg) == D2_ERROR)
					{
            				return_code->retcode = 
						CONVERR(d20_head->d2_retcode);
            				return_code->errclass = 
						CONVERR(d20_head->d2_errclass);
            				return_code->errvalue = 
						d20_head->d2_errvalue;
            				ret_value=D2_ERROR;
            				break;
         				}
				}
			else
				{
				/* alias object under ROOT should not be */
				/* involved in chnam operation.	         */

            			return_code->retcode  = PERMERR;
            			return_code->errclass = SYSERR;
				return_code->errvalue = D2_ON_ALIAS_NOT_ALLOWED;
            			ret_value=D2_ERROR;
            			break;
				}
			}

		/* Get the index of DSA   */
         	index=d21t046_dsa_table(num_dsa, mk_attr);

         	if (index>=D20_INT_DN_NO) 
			{
            		return_code->retcode  = PERMERR;
            		return_code->errclass = SYSERR;
            		return_code->errvalue = DS_ALLOC_ERROR;
            		ret_value=D2_ERROR;
            		break;
         		}

         	*prdn=num_rdn;

		if ((int)strlen(object) <= (int)strlen(new_dn))
         		num_rdn->rdnn = d20_beg + (*ptr)->d20_mem_off;
		else
         		num_rdn->rdnn = d20_beg + (*ptr)->d20_mem_off +
					strlen(new_dn) + 1;

         	num_rdn->dsa_in[index]=1;
         	num_rdn++;
         	prdn++;

      		} /* if */

   	} /* for */

if (ret_value != D2_NOERROR) 
	{
      	d20_111_free((char *) dn, (char *) pdn, names, (char *) first_num_rdn,
		     (char *) first_prdn, at_mem, at_mem2,
		     (char *) first_area_aux, at_val_area, NULL);
      	d20_111_free((char *) dn2, (char *) pdn2, name2, (char *) b_conflicts,
		     NULL, NULL, NULL, NULL, NULL, NULL);

        close(fd2);
        unlink(file2);
      	close(fd);
      	unlink(file);
      	unlink(file3);

      	d20_bid = save_bid;
	/*
      	d21d009_errlog("EXIT  d21_15_chnam", return_code);
	*/
      	return(ret_value);
   	}

*prdn=NULL;
n_elem=prdn-first_prdn;           /* Number of elements into table */
qsort(first_prdn, (size_t) n_elem, (size_t) sizeof(String),
    (int (*)(const void *, const void *))d21t043_compare_rdn);
 
   /*******************************************************************/
   /*               Add Objects in the needed DSA's                   */
   /*******************************************************************/
 
d20_head=&head;
head.d2_version = DS_V02;
head.d2_retcode = head.d2_errclass = head.d2_errvalue = D2_NO_ERR;
d20_beg = names;                   /* Initial address of the DN's    */
ptr=(D20_offset **)pdn;

/* Get index of the DSA table     */
index=d21t046_dsa_table(num_dsa, curr_dsa);

for(i=0; i < dn_no; i++, ptr++) 
	{
      	strcpy(object, d20_beg + (*ptr)->d20_mem_off);

       	/* If the object belongs to the subtree */
      	if (d21t042_filter_obj(object, old_dn) == D2_REAL_OBJ) 
		{

               	/* Build the new DN             */
         	d21t045_build_dn(old_dn, new_dn, object, n_name);

		if (((*ptr)->d20_state & D21_ALIAS_OBJ) == 0) 
			{
			/* object is not an alias.	*/

			/* Looking if the new object exists     */
         		found=d21t040_search_dn(pdn2, dn_no2, name2,
					(Name_string) n_name);

			/* if new object exists, get its MK, otherwise get  */
			/* MK of old object.				    */
         		if (found != NULL) 
				{
				/* object is found under new parent.	*/
				/* get master knldg. of the new object.	*/

				if (((*found)->d20_state & D21_ALIAS_OBJ) == 0) 
					{
					/* new object is not an alias.	*/
            				if (d21t008_get_attr(fd2,*found, 
						at_mem2, &min_pos2, &max_pos2, 
						MK, &mk_attr, &mk_len, name2) 
						== D2_ERROR) 
						{
            					return_code->retcode = 
						CONVERR(d20_head->d2_retcode);
            					return_code->errclass = 
						CONVERR(d20_head->d2_errclass);
            					return_code->errvalue = 
						d20_head->d2_errvalue;
            					ret_value=D2_ERROR;
            					break;
						}
					}
				else
					{
					/* object is an alias, read master  */
					/* knowledge of parent.		    */
					/* if object is not directly under  */
					/* root of DIT ,parent must exist   */
					/* in the search result.   	    */

            				if (d21t033_parent_object_name(n_name, 
							auxobj) != FALSE)
						{
         					found=d21t040_search_dn(pdn2, 
							dn_no2, name2,
						        (Name_string) auxobj);

         					if (d21t008_get_attr(fd2,*found,							at_mem2, &min_pos2, 
							&max_pos2, MK, &mk_attr,
							 &mk_len, name2)
								 == D2_ERROR)
							{
            						return_code->retcode = 
						CONVERR(d20_head->d2_retcode);
            						return_code->errclass = 
						CONVERR(d20_head->d2_errclass);
            						return_code->errvalue = 
							d20_head->d2_errvalue;
            						ret_value=D2_ERROR;
            						break;
         						}
						}
					else
						{
						/* alias object under ROOT */
						/* should not be involved  */
						/* in chnam operation.	   */

            					return_code->retcode  = PERMERR;
            					return_code->errclass = SYSERR;
						return_code->errvalue = 
						D2_ON_ALIAS_NOT_ALLOWED;
            					ret_value=D2_ERROR;
            					break;
						}
					}
				}
         		else 
				{
				/* object is not found under new parent.     */
				/* get master knldg. of the old object.	     */

				/* old object is not an alias.	*/
            			if (d21t008_get_attr(fd, *ptr, 
						at_mem, &min_pos, &max_pos, 
						MK, &mk_attr, &mk_len, d20_beg)
								 == D2_ERROR)
					{
            				return_code->retcode = 
						CONVERR(d20_head->d2_retcode);
            				return_code->errclass = 
						CONVERR(d20_head->d2_errclass);
            				return_code->errvalue = 
						d20_head->d2_errvalue;
            				ret_value=D2_ERROR;
            				break;
					}
				}
			} /* object being handled is not an alias.	*/
		else
			{
			/* object is an alias. Alias object should be   */
			/* added in the master DSA of new parent.	*/
			/* If new parent exists in new subtree, get its */
			/* MK, otherwise get MK of old parent.		*/
            		if (d21t033_parent_object_name(n_name, 
							auxobj) != FALSE)
				{
				/* look if new parent exists in new subtree. */
         			found = d21t040_search_dn(pdn2, dn_no2, name2, 
							  (Name_string) auxobj);
				
				if (found != NULL)
					{
					/* new parent exists in new subtree. */

					if (((*found)->d20_state & 
							D21_ALIAS_OBJ) == 0) 
						{
						/* new parent is not an alias.*/
						if (d21t008_get_attr(
							fd2,*found, at_mem2, 
							&min_pos2, &max_pos2, 
							MK, &mk_attr, &mk_len, 
							name2) == D2_ERROR) 
							{
            						return_code->retcode = 
						CONVERR(d20_head->d2_retcode);
            						return_code->errclass = 
						CONVERR(d20_head->d2_errclass);
            						return_code->errvalue = 
							d20_head->d2_errvalue;
            						ret_value=D2_ERROR;
            						break;
							}
						}
					else
						{
						/* new parent object is alias.*/
						/* get new parent's parent MK.*/
            					if (d21t033_parent_object_name(
						auxobj, par_auxobj) != FALSE)
							{
         						found = 
							d21t040_search_dn(pdn2,
							dn_no2, name2, 
							(Name_string) par_auxobj);

							if (d21t008_get_attr(
							fd2,*found, at_mem2, 
							&min_pos2, &max_pos2, 
							MK, &mk_attr, &mk_len, 
							name2) == D2_ERROR) 
								{
            						return_code->retcode = 
						CONVERR(d20_head->d2_retcode);
            						return_code->errclass = 
						CONVERR(d20_head->d2_errclass);
            						return_code->errvalue = 
							d20_head->d2_errvalue;
            						ret_value=D2_ERROR;
            						break;
								}
							}
						else
							{
						/* alias object under ROOT */
						/* should not be involved  */
						/* in chnam operation.	   */

            					return_code->retcode  = PERMERR;
            					return_code->errclass = SYSERR;
						return_code->errvalue = 
							D2_ON_ALIAS_NOT_ALLOWED;
            					ret_value=D2_ERROR;
            					break;
							}
						}
					}
				else
					{
					/* new parent does not exist, look */
					/* for MK of old parent.	   */
            				if (d21t033_parent_object_name(object, 
							auxobj) != FALSE)
						{
						/* parent must exist in the */
						/* search result.	    */
         					found = d21t040_search_dn(pdn, 
							dn_no, names,
							(Name_string) auxobj);

            					if (d21t008_get_attr(fd,
							*found, at_mem, 
							&min_pos, &max_pos, MK,
							&mk_attr, &mk_len, 
							d20_beg) == D2_ERROR)
							{
            						return_code->retcode = 
							CONVERR(d20_head->
								d2_retcode);
            						return_code->errclass = 
							CONVERR(d20_head->
								d2_errclass);
            						return_code->errvalue = 
							d20_head->d2_errvalue;
            						ret_value = D2_ERROR;
            						break;
         						}
						}
					else
						{
						/*this condition should never */
						/* occur.		      */
						}
					}
				}
			else
				{
				/* this is the case when chnam is called for */
				/* an alias object to an object under ROOT.  */

         			found = d21t040_search_dn(pdn2, dn_no2, name2, 
							  (Name_string) n_name);

				/* n_name must exist in the search result. */
				if (((*found)->d20_state & 
							D21_ALIAS_OBJ) == 0) 
					{
					/* new parent is not an alias.*/
					if (d21t008_get_attr(
							fd2,*found, at_mem2, 
							&min_pos2, &max_pos2, 
							MK, &mk_attr, &mk_len, 
							name2) == D2_ERROR) 
						{
            					return_code->retcode = 
						CONVERR(d20_head->d2_retcode);
            					return_code->errclass = 
						CONVERR(d20_head->d2_errclass);
            					return_code->errvalue = 
							d20_head->d2_errvalue;
            					ret_value=D2_ERROR;
            					break;
						}
					}
				else
					{
					/* alias object under ROOT */
					/* should not be involved  */
					/* in chnam operation.	   */

            				return_code->retcode  = PERMERR;
            				return_code->errclass = SYSERR;
					return_code->errvalue = 
							D2_ON_ALIAS_NOT_ALLOWED;
            				ret_value=D2_ERROR;
            				break;
					}
				}
			}


         	if (ret_value == D2_ERROR) 
			{
            		d20_111_free((char *) dn, (char *) pdn, names,
			(char *) first_num_rdn, (char *) first_prdn, at_mem,
			at_mem2, (char *) first_area_aux, at_val_area, NULL);
            		d20_111_free((char *) dn2, (char *) pdn2, name2,
			(char *) b_conflicts, NULL, NULL, NULL, NULL, NULL,
			NULL);
            		return_code->retcode = CONVERR(d20_head->d2_retcode);
            		return_code->errclass = CONVERR(d20_head->d2_errclass);
            		return_code->errvalue = d20_head->d2_errvalue;

               		close(fd2);
               		unlink(file2);
            		close(fd);
            		unlink(file);
			unlink(file3);

            		if (curr_id > 0) 
               			d20__unbind(curr_id);

            		d20_bid = save_bid;
			/*
	    		d21d009_errlog("EXIT  d21_15_chnam", return_code);
			*/
            		return(ret_value);
         		}

      		/* Bind to a new DSA            */
         	if (d21t047_bind_dsa(curr_dsa, mk_attr, &curr_id, dir_id, 
							&c_arg) == D2_ERROR) 
			{
            		d20_111_free((char *) dn, (char *) pdn, names,
			(char *) first_num_rdn, (char *) first_prdn, at_mem,
			at_mem2, (char *) first_area_aux, at_val_area, NULL);
            		d20_111_free((char *) dn2, (char *) pdn2, name2,
			(char *) b_conflicts, NULL, NULL, NULL, NULL, NULL,
			NULL);
            		return_code->retcode = CONVERR(d20_head->d2_retcode);
            		return_code->errclass = CONVERR(d20_head->d2_errclass);
            		return_code->errvalue = d20_head->d2_errvalue;

               		close(fd2);
               		unlink(file2);
            		close(fd);
            		unlink(file);
            		unlink(file3);

            		d20_bid = save_bid;
			/*
	    		d21d009_errlog("EXIT  d21_15_chnam", return_code);
			*/
            		return(D2_ERROR);
         		}

   		index=d21t046_dsa_table(num_dsa, mk_attr);

		/* Looking if the new object exists     */
         	found=d21t040_search_dn(pdn2, dn_no2, name2,
				(Name_string) n_name);

		if (found == NULL)
			{
			/* only in case when new object doesn't exist, try */
			/* try to add superior objects as shadows.	   */

			/* Ptr to area of RDNs not added  */
         		area_aux = first_area_aux; 

			/* Area of RDNs not added         */
         		area_rdn = first_area_rdn; 

         		end_loop = FALSE;

			/* Name of the object to add      */
         		strcpy(auxobj, n_name);    

         		while (!end_loop) 
				{
                       		/* Get the parent object               */
            			if (d21t033_parent_object_name(auxobj,object) 
								== FALSE)
					/* The end of the distinguished name  */
               				break; 

				if ((int)strlen(object) <= (int)strlen(new_dn))
            				par_name.rdnn = object;
				else
            				par_name.rdnn = object + 
							strlen(new_dn) + 1;

            			*par_n = (D20_rdn_tbl *)&par_name;
            			p_num_rdn = (D20_rdn_tbl **)bsearch(par_n, 
					    first_prdn, (size_t) n_elem,
					    (size_t) sizeof(String), 
					    (int (*)(const void *,
					    const void *))
					    d21t043_compare_rdn);

            			if (p_num_rdn == NULL || 
				   (*p_num_rdn)->dsa_in[index]==0) 
					{
                               		/* The object does not exists    */
                               		/* we will add a shadow entry    */

               				head.d2_retcode = head.d2_errclass = 
					head.d2_errvalue = D2_NO_ERR;

               				d21t049_hdl_obj(fd, pdn, names, dn_no, 
					at_mem, &min_pos, &max_pos, fd2, pdn2, 
					name2, dn_no2, at_mem2, &min_pos2,
					&max_pos2, at_val_area, old_dname, 
					new_dname, (Name_string) object,
					(Name_string) curr_dsa);

               				switch(head.d2_errvalue) 
						{/* Can not be created        */
                  				case D2_NO_ERR:        
						/* The object has been added */
                               		         /* Add rdn into table        */
		  				case D2_ALREADY_EXISTS:
						/* The object exists         */
							d20_beg1 = names;
							d20_beg2 = name2;
                     					d21t048_ins_rdn(
							first_prdn, 
							first_num_rdn,
							(signed32 *) &n_elem,
							object, index, pdn,
							dn_no, pdn2, dn_no2,
							old_dn, new_dn);
                     					end_loop = TRUE;
                  					break;

                  				case DS_ADD_ERR:   
						/*Parent object does not exist*/
                        			/* RDN not inserted is saved  */
						/* into this area to be       */
						/* inserted later	      */

							/* Get the last RDN   */
         						d21t041_get_rdn(object,
									rdn);

                     					area_rdn += D2_NP_LEN;
                     					*area_aux = area_rdn;
                     					strcpy(*area_aux, rdn);
                     					area_aux++;
                  					break;

                  				default:           
						/* Error adding the object    */
                     					return_code->retcode  = 
						CONVERR(head.d2_retcode);
                     					return_code->errclass = 
						CONVERR(head.d2_errclass);
                     					return_code->errvalue = 
							head.d2_errvalue;
                     					ret_value = D2_ERROR;
               					}

               				if (ret_value != D2_NOERROR) 
                  				break;

            				} /*if rdn does not exist in the table*/
            			else 
					/* The object exists */
               				end_loop = TRUE;   

				/* Next object to be inserted*/
            			strcpy(auxobj, object);  

         			} /* while rdn not exists */

        	 	if (ret_value!=D2_NOERROR) 
				/* If error breaks the FOR loop  */
            			break;

			/* The last object inserted      */
         		strcpy(full_name, object);  
 
         		while(area_aux != first_area_aux) 
				{
            			area_aux--;
            			sprintf(full_name, "%s%c%s", full_name, 
							D2_RDN_SEP, *area_aux);

				/* we will add a shadow entry    */
	
       	        		if (d21t049_hdl_obj(fd,pdn,names,dn_no,at_mem,
					&min_pos, &max_pos,
					fd2,pdn2,name2,dn_no2,at_mem2,&min_pos2,
					&max_pos2, at_val_area, old_dname,
					new_dname, (Name_string) full_name,
					(Name_string) curr_dsa) == D2_NOERROR)
					{
       	                        	/* Add rdn into table of rdns    */
					d20_beg1 = names;
					d20_beg2 = name2;
       	        			d21t048_ins_rdn(first_prdn,
						first_num_rdn, 
						(signed32 *) &n_elem, full_name,
						index, pdn, dn_no, pdn2, dn_no2,
						old_dn, new_dn);
       	     				}
            			else 
					{
               				return_code->retcode = 
						CONVERR(head.d2_retcode);
               				return_code->errclass = 
						CONVERR(head.d2_errclass);
               				return_code->errvalue = 
						head.d2_errvalue;
               				ret_value = D2_ERROR;
               				break;
            				}

         			} /* while */

         		if (ret_value!=D2_NOERROR) 
				/* If error breaks the FOR loop  */
            			break;
			} /* end of case when new object does not exist.    */

 
        /**************************************************************/
        /*                Adding the real object as master            */
        /**************************************************************/
 
         	if (found == NULL) 
			{ /* The object to add does not exist  */
			
			d20_beg1 = d20_beg;

            		if (d21t003_add_appnd(fd, *ptr, at_mem, &min_pos, 
			    &max_pos, OLD_ACL, (Name_string)new_dname, 
			    (Name_string)old_dname, 
			    TRUE, (Name_string) curr_dsa, 0, (char *)NULL, 
			    at_val_area, CHANGE_NAM)== D2_NOERROR)
				{
                               	/* Add rdn into table of rdns    */
				d20_beg1 = names;
				d20_beg2 = name2;
               			d21t048_ins_rdn(first_prdn, first_num_rdn, 
						(signed32 *) &n_elem, n_name,
						index, pdn, dn_no, pdn2, dn_no2,
						old_dn, new_dn);
               			*(b_conflicts + i) = FALSE;
            			}
            		else 
				{
               			return_code->retcode = CONVERR(d20_head->
								d2_retcode);
               			return_code->errclass = CONVERR(d20_head->
								d2_errclass);
               			return_code->errvalue = d20_head->d2_errvalue;
               			ret_value = D2_ERROR;
               			break;
            			}
         		} /* if NOT found */
         	else 
			{
            		if (overwrite == TRUE) 
				{
				d20_beg1 = names;
				d20_beg2 = name2;

               			if (d21t004_modify_appnd(fd, *ptr, at_mem,
				    &min_pos, &max_pos,
				    fd2, *found, at_mem2, &min_pos2, &max_pos2,
				    OLD_ACL, (Name_string)new_dname, 
				    (Name_string)old_dname,
				    TRUE, (char *)NULL, 0,
				    (Name_string) curr_dsa,overwrite,
				    b_conflicts + i, at_val_area,
				    CHANGE_NAM)==D2_NOERROR) 
					{
                                     	/* Add rdn into table of rdns    */
					min_pos2 = max_pos2 = 0;
					d20_beg1 = names;
					d20_beg2 = name2;
                  			d21t048_ins_rdn(first_prdn, 
					first_num_rdn, (signed32 *) &n_elem,
					n_name, index, pdn, dn_no, pdn2, dn_no2,
					old_dn, new_dn);
               				}
               			else 
					{
					min_pos2 = max_pos2 = 0;
                  			return_code->retcode = CONVERR(head.
								d2_retcode);
                  			return_code->errclass = CONVERR(head.
								d2_errclass);
                  			return_code->errvalue=head.d2_errvalue;
                  			ret_value = D2_ERROR;
                  			break;
               				}
            			} /* if overwrite */
         		}

      		} /* if the object belongs to the subtree */
      	else 
         	*(b_conflicts + i) = FALSE;

   	} /* for all objects */
 
if (curr_id > 0)
	d20__unbind(curr_id);

if (ret_value != D2_NOERROR) 
	{
      	d20_111_free((char *) dn, (char *) pdn, names, (char *) first_num_rdn,
		     (char *) first_prdn, at_mem, at_mem2,
		     (char *) first_area_aux, at_val_area, NULL);
      	d20_111_free((char *) dn2, (char *) pdn2, name2, (char *) b_conflicts,
		     NULL, NULL, NULL, NULL, NULL, NULL);

        close(fd2);
        unlink(file2);
      	close(fd);
      	unlink(file);
      	unlink(file3);

      	d20_bid = save_bid;
	/*
      	d21d009_errlog("EXIT  d21_15_chnam", return_code);
	*/
      	return(ret_value);
   	}
 
   /**************************************************************/
   /*                Deleting as many objects as posible         */
   /**************************************************************/
 
min_pos = max_pos = 0;
return_code->retcode  = return_code->errclass = NOERR;
return_code->errvalue = D2_NO_ERR;

/* start deleting objects from the first dsa (bind_id).	*/
curr_id = bind_id;
strcpy(curr_dsa, bind_dsa);

for (; ;) 
	{   
	strcpy(norm_curr_dsa, curr_dsa);
	d20_norm(norm_curr_dsa, strlen(norm_curr_dsa));

      	c_arg.d2_serv_cntrls |= D2_LOCALSCOPE;
      	c_arg.d2_serv_cntrls &= ~D2_NOCACHE;

      	ret_value = d21_10_delst(curr_id, return_code, version,
			(Name_string) old_dn, c_arg.d2_serv_cntrls, 
			(Name_string **) &not_del);

       	/* There are some objects not deleted */
      	if (ret_value == D2_ERROR && return_code->errvalue == DS_DEL_ERR) 
		{
	 	return_code->retcode  = return_code->errclass = NOERR;
         	return_code->errvalue = D2_NO_ERR;
         	ret_value = D2_NOERROR;
         	ptr_del = not_del;

         	while(*ptr_del != NULL) 
			/* Looking for the last one     */
            		ptr_del++;

         	if (ptr_del != not_del) 
			{
            		ptr_del--;  /* This is the not deleted object   */
                                    /* Looking for the object to delete */

            		found = d21t040_search_dn(pdn, dn_no, names, 
					(Name_string) *ptr_del);

            		if (found == NULL) 
               			break;

			/* Get the Master K. attribute      */
            		if (d21t008_get_attr(fd, *found, at_mem, &min_pos, 
					&max_pos, MK,&mk_attr, &mk_len, d20_beg)
                		== D2_ERROR) 
				{
               			return_code->retcode = CONVERR(d20_head->
								d2_retcode);
               			return_code->errclass = CONVERR(d20_head->
								d2_errclass);
               			return_code->errvalue = d20_head->d2_errvalue;
               			ret_value = D2_ERROR;
               			break;
            			}

			/* norm name for master-knowledge attr. */
			strcpy(norm_mk_attr, mk_attr);
			d20_norm(norm_mk_attr, strlen(norm_mk_attr));
			/* if object is master, return delete error.	*/
            		if (!strcmp(norm_mk_attr, norm_curr_dsa)) 
				{
      				return_code->retcode  = PERMERR;
      				return_code->errclass = SYSERR;
         			return_code->errvalue = DS_DEL_ERR;
                     		ret_value = D2_ERROR;
                     		break;
                  		}

			/* Bind to a new DSA            */
            		if (d21t047_bind_dsa(curr_dsa, mk_attr, &curr_id, 
				dir_id, &c_arg) == D2_ERROR) 
				{
               			return_code->retcode = CONVERR(d20_head->
								d2_retcode);
               			return_code->errclass = CONVERR(d20_head->
								d2_errclass);
               			return_code->errvalue = d20_head->d2_errvalue;
               			ret_value = D2_ERROR;
               			break;
            			}

	/*
   			index=d21t046_dsa_table(num_dsa, mk_attr);
	*/

            		free(not_del);

         		} /* if */

      		}
      	else 
		{
         	if (!strcmp(curr_dsa, bind_dsa)) 
			{
            		if (return_code->errvalue == DS_NFOUND) 
				{
	       			return_code->retcode = return_code->errclass = 
									NOERR;
               			return_code->errvalue = D2_NO_ERR;
               			ret_value = D2_NOERROR;
            			}

            		break; /* The delete process has finished   */
         		}

         	if (ret_value == D2_ERROR) 
            		break; /* It was an unexpected  error       */

		/* Bind to the original DSA     */
         	if (d21t047_bind_dsa(curr_dsa, bind_dsa, &curr_id, 
		  		     		dir_id, &c_arg)==D2_ERROR) 
			{
            		return_code->retcode = CONVERR(d20_head->d2_retcode);
            		return_code->errclass = CONVERR(d20_head->d2_errclass);
            		return_code->errvalue = d20_head->d2_errvalue;
            		ret_value = D2_ERROR;
            		break;
         		}

	/*
   		index=d21t046_dsa_table(num_dsa, bind_dsa);
	*/

      		}

   	} /* for EVER */
 
if (ret_value == D2_NOERROR) 
	{
      	d20_beg1 = names;          /* Initial address of the old DN's   */
      	d20_beg2 = name2;          /* Initial address of the new DN's   */

      	if (d21t068_conflicts(dn_no, pdn, conflicts, b_conflicts, old_dn, 
			      new_dn) == D2_ERROR) 
		{
         	return_code->retcode  = PERMERR;
         	return_code->errclass = SYSERR;
         	return_code->errvalue = DS_ALLOC_ERROR;
         	ret_value = D2_ERROR;
      		}

      	if (d21t065_change_dnlist((char *) old_dname, (char *) new_dname,
			curr_id, return_code) == D2_ERROR) 
         	ret_value = D2_ERROR;
   	}

d20_111_free((char *) dn, (char *) pdn, names, (char *) first_num_rdn,
		(char *) first_prdn, at_mem, at_mem2, (char *) first_area_aux,
		at_val_area, NULL);
d20_111_free((char *) dn2, (char *) pdn2, name2, (char *) not_del,
		(char *) b_conflicts, NULL, NULL, NULL, NULL, NULL);

if (curr_id > 0) 
      	d20__unbind(curr_id);

close(fd2);
unlink(file2);
close(fd);
unlink(file);
unlink(file3);

d20_bid = save_bid;
/*
d21d009_errlog("EXIT  d21_15_chnam", return_code);
*/
return(ret_value);

}

 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t040_search_dn()                                   */
/*                                                                    */
/* AUTHOR:      J. Pulpillo, D 525 (CDS)                              */
/* DATE:        27.09.89                                              */
/*                                                                    */
/* SYNTAX:  D20_offset **d21t040_search_dn(first,num_dn,beg2,object)  */
/*                                                                    */
/*              D20_offset  **first;                                  */
/*              signed16      num_dn;                                 */
/*              char         *beg2;                                   */
/*              Name_string   object;                                 */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function returns the ptr to the dn offset of the */
/*              required object                                       */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              D20_offset  **first;  Ptr to the array of ptrs.       */
/*              signed16      num_dn; Number of dn.                   */
/*              char         *beg2;   Ptr to the data area            */
/*              Name_string   object; Object to be searched           */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              Ptr to the dn if it was found                         */
/*              Null if the object does not exist                     */
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
 
 D20_offset **d21t040_search_dn(
   D20_offset **first,
   signed16     num_dn,
   char        *beg2,
   Name_string  object)
 
{
   D20_offset      dist_n;
   D20_offset    **found, *auxptr[1];
   unsigned32      nu_elem;
char	norm_object[D2_DNL_MAX + 1];

strcpy(norm_object, (char *)object);
d20_norm(norm_object, strlen(norm_object));
 
 
   if (num_dn==0) {        /*  The parent object of the new dn is Root*/
      found=NULL;
   }
   else {
      dist_n.d20_mem_off=0;     /*  Look for the address of the object*/
      *auxptr=&dist_n;
      d20_beg1=(char *)norm_object;
      d20_beg2=beg2;
      nu_elem=num_dn;
      found=(D20_offset **) bsearch(auxptr, first, (size_t) nu_elem,
		(size_t) sizeof(String),
		(int (*)(const void *, const void *)) d20_101_compare_dn);
   }
   return(found);
}
 
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t041_get_rdn()                                     */
/*                                                                    */
/* AUTHOR:      J. Pulpillo, D 525 (CDS)                              */
/* DATE:        27.09.89                                              */
/*                                                                    */
/* SYNTAX:      void d21t041_get_rdn(obj,rdn)                        */
/*                                                                    */
/*              char    *obj;                                         */
/*              char    *rdn;                                         */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function returns the last relative distinguished */
/*              name of an object and its object-type                 */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              char    *obj;       name of the object                */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              char    *rdn;       last relative distinguished name  */
/*                                                                    */
/* RETURNVALUE:                                                       */
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
 
 static void d21t041_get_rdn(
   char *obj,
   char *rdn)
 
{
   char  *end_obj;
 
   end_obj = obj + strlen(obj);

   for ( ;end_obj != obj; end_obj--)
      if (*end_obj == D2_RDN_SEP)
	  break;

   if (end_obj != obj)
      end_obj++;                          /* Begin of the RDN & type */

   strcpy(rdn,end_obj);
}
 
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t042_filter_obj()                                  */
/*                                                                    */
/* AUTHOR:      J. Pulpillo, D 525 (CDS)                              */
/* DATE:        25.10.89                                              */
/*                                                                    */
/* SYNTAX:      signed16 d21t042_filter_obj(obj, subtree)             */
/*                                                                    */
/*              char    *obj;                                         */
/*              char    *subtree;                                     */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function looks if the object belongs to the      */
/*              subtree or is a parent object of the subtree.         */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              char    *obj;       name of the object                */
/*              char    *subtree;   name of subtree                   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              REAL_OBJ      :  the object belongs to the subtree    */
/*              PARENT_OBJ    :  the object is a parent object of the */
/*				 subtree			      */
/*              EXTERN_OBJ    :  the object does not belong to the    */
/*				 subtree   			      */
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
 
 signed16 d21t042_filter_obj(
   char *obj,
   char *subtree)
 
{
 
   for( ;*obj!=D2_EOS && *subtree!=D2_EOS; obj++, subtree++) {
      if (*obj != *subtree) {
         return(D2_EXTERN_OBJ);
      }
   }

   /* PARENT_OBJ if object is parent, 				*/
   /* REAL_OBJ if object is son or both are equals   		*/

   return((*subtree==D2_EOS)?D2_REAL_OBJ:D2_PARENT_OBJ);
}
 
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t043_compare_rdn()                                 */
/*                                                                    */
/* AUTHOR:      J. Pulpillo, D 525 (CDS)                              */
/* DATE:        25.10.89                                              */
/*                                                                    */
/* SYNTAX:      int d21t043_compare_rdn(rdn1, rdn2)                   */
/*                                                                    */
/*              D20_rdn_tbl *rdn1;                                    */
/*              D20_rdn_tbl *rdn2;                                    */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function compares the RDN of two objects.        */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              D20_rdn_tbl *rdn1;                                    */
/*              D20_rdn_tbl *rdn2;                                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
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
 
 static signed32 d21t043_compare_rdn(
   D20_rdn_tbl **rdn1,
   D20_rdn_tbl **rdn2)
 
{
return(strcmp((*rdn1)->rdnn, (*rdn2)->rdnn));
}
 
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t044_get_memory()                                  */
/*                                                                    */
/* AUTHOR:      J. Pulpillo, D 525 (CDS)                              */
/* DATE:        25.10.89                                              */
/*                                                                    */
/* SYNTAX:      Bool d21t044_get_memory(dn_no, num_rdn, prdn,         */
/*                     attr_len, at_mem,attr_len2, at_mem2, area_aux))*/
/*                                                                    */
/*              signed32          dn_no;                              */
/*              D20_rdn_tbl     **num_rdn;                            */
/*              D20_rdn_tbl     **prdn;                               */
/*              signed32          attr_len;                           */
/*              char            **at_mem;                             */
/*              signed32          attr_len2;                          */
/*              char            **at_mem2;                            */
/*              char            **area_aux;                           */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function gets memory for the all the tables      */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              signed32          dn_no;                              */
/*              D20_rdn_tbl     **num_rdn;                            */
/*              D20_rdn_tbl     **prdn;                               */
/*              signed32          attr_len;                           */
/*              char            **at_mem;                            */
/*              signed32          attr_len2;                          */
/*              char            **at_mem2;                            */
/*              char            **area_aux;                           */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              ERROR      if there was an allocating memory error    */
/*              NOERROR    if the operation was succesfully           */
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
 
 signed16 d21t044_get_memory(
   signed32      dn_no,
   D20_rdn_tbl **num_rdn,
   D20_rdn_tbl **prdn,
   signed32      attr_len,
   char        **at_mem,
   signed32      attr_len2,
   char        **at_mem2,
   char        **area_aux)
 
{
   signed32 length;
                         /* Size of area used for to save rdn         */
   length=(dn_no * sizeof(char **)) + (dn_no * D2_NP_LEN);
 
                        /* This ensures the space will be zero filled */
   if ((*num_rdn =(D20_rdn_tbl *)calloc(dn_no, sizeof(D20_rdn_tbl)))
             == (D20_rdn_tbl *) NULL) {
      return(D2_ERROR);
   }
   if ((*prdn = (D20_rdn_tbl *) malloc(dn_no * sizeof(D20_rdn_tbl *)))
             == (D20_rdn_tbl *) NULL) {
      return(D2_ERROR);
   }
   if ((*area_aux =  (char *)malloc(length)) == (char *) NULL) {
      return(D2_ERROR);
   }
                                   /* get memory for the attributes 1 */
   attr_len += D2_ALIGN(D2_DNL_MAX);
   if (attr_len<D20_BUFSIZ) {
      attr_len=D20_BUFSIZ;
   }
   if ((*at_mem = (char *)malloc(attr_len)) == (char *) NULL) {
      return(D2_ERROR);
   }
                                   /* get memory for the attributes 2 */
   attr_len2 += D2_ALIGN(D2_DNL_MAX);
   if (attr_len2<D20_BUFSIZ) {
      attr_len2=D20_BUFSIZ;
   }
   if ((*at_mem2 = (char *)malloc(attr_len2)) == (char *) NULL) {
      return(D2_ERROR);
   }
   return(D2_NOERROR);
}
 
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t045_build_dn()                                    */
/*                                                                    */
/* AUTHOR:      J. Pulpillo, D 525 (CDS)                              */
/* DATE:        25.10.89                                              */
/*                                                                    */
/* SYNTAX:      void  d21t045_build_dn(old_dn,new_dn,name,n_name)     */
/*                                                                    */
/*              char    *old_dn;                                      */
/*              char    *new_dn;                                      */
/*              char    *name;                                        */
/*              char    *n_name;                                      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function builds the new DN of an object based on */
/*              the DN of the new parent object                       */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              char    *old_dn;    Name of the old parent object     */
/*				    (Normalized)		      */
/*              char    *new_dn;    Name of the new parent object     */
/*				    (Normalized)		      */
/*              char    *name;      Name of the object to be renamed  */
/*				    (Not Normalized)		      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              char    *n_name;    New name of the object            */
/*                                                                    */
/* RETURNVALUE:                                                       */
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
 
 void  d21t045_build_dn(
   char    *old_dn,
   char    *new_dn,
   char    *name,
   char    *n_name)
 
{
   char        *auxptr;
   char       norm_name[D2_DNL_MAX+1]; 
   char       norm_old_dn[D2_DNL_MAX+1]; 
   char       norm_new_dn[D2_DNL_MAX+1]; 
 
strcpy(norm_name, name);
d20_norm(norm_name, strlen(norm_name)); /* normalized                    */

strcpy(norm_new_dn, new_dn);
d20_norm(norm_new_dn, strlen(norm_new_dn)); /* normalized                    */

strcpy(norm_old_dn, old_dn);
d20_norm(norm_old_dn, strlen(norm_old_dn)); /* normalized                    */

   if (d21t042_filter_obj(norm_name,norm_old_dn) != D2_REAL_OBJ) {
      strcpy(n_name,name);   /* The object not belongs to the subtree */
                             /* or the object is a parent object      */
      return;
   }

   if (strcmp(norm_old_dn,norm_name)==0) {/* Is the new parent object */
      strcpy(n_name,new_dn);              /* New parent object        */
   }
   else {
      auxptr=name+strlen(old_dn);         /* Ptr to the begin of RDN  */
      strcpy(n_name,new_dn);              /* New parent object        */
      strcat(n_name,auxptr);              /* RDN of the object        */
   }
}

 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t046_dsa_table()                                   */
/*                                                                    */
/* AUTHOR:      J. Pulpillo, D 525 (CDS)                              */
/* DATE:        25.10.89                                              */
/*                                                                    */
/* SYNTAX:      signed16 d21t046_dsa_table(num_dsa, look_dsa)         */
/*                                                                    */
/*              char    *num_dsa;                                     */
/*              char    *look_dsa;                                    */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function looks for a DSA into DSA table. If it   */
/*              exists returns the index of the table, else inserts   */
/*              this DSA into the table and returns the new index     */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              char    *num_dsa;   Ptr to first element of the table */
/*              char    *look_dsa;  Name of the DSA to look for       */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              The index number of the DSA into the table            */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read-only):                                         */
/* GLOBAL  DATA  (updated):                                           */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/
 
 signed16 d21t046_dsa_table(
   D20_dsa_tbl  num_dsa[],
   char        *look_dsa)
 
{
   signed16 index;
   char     norm_num_dsa[D2_DNL_MAX+1];
   char     norm_look_dsa[D2_DNL_MAX+1];

   /* norm name for dsa to look for */
   strcpy(norm_look_dsa, look_dsa);
   d20_norm(norm_look_dsa, strlen(norm_look_dsa));

                                    /* Looking for the index of DSA   */
   for(index=0 ; index<D20_INT_DN_NO ; index++) {
      /* norm name for dsaname */
      strcpy(norm_num_dsa, num_dsa[index].dsan);
      d20_norm(norm_num_dsa, strlen(norm_num_dsa));

      if (strcmp(norm_num_dsa,norm_look_dsa)==0) {
         break;                      /* The DSA name already exists   */
      }
      if (num_dsa[index].ind==0) {
         strcpy(num_dsa[index].dsan,look_dsa);
         num_dsa[index].ind=1;
      break;                         /* New DSA name inserted         */
      }
   } /* for */
   return(index);
}
 
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t047_bind_dsa()                                    */
/*                                                                    */
/* AUTHOR:      J. Pulpillo, D 525 (CDS)                              */
/* DATE:        25.10.89                                              */
/*                                                                    */
/* SYNTAX:      signed16 d21t047_bind_dsa(curr_dsa, new_dsa, new_bind,*/
/*                            dir_id, c_arg)                          */
/*                                                                    */
/*              char       *curr_dsa;                                 */
/*              char       *new_dsa;                                  */
/*              signed16   *new_bind;                                 */
/*              signed16    dir_id;                                   */
/*              D2_c_arg   *c_arg;                                    */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function looks if the new DSA is the binded DSA  */
/*              or not, and and makes a bind to the new DSA           */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              char       *curr_dsa;  Name of the current DSA binded */
/*              char       *new_dsa;   Name of the new DSA to bind    */
/*              signed16    dir_id;    Identifier of installation used*/
/*              D2_c_arg   *c_arg;     Common arguments               */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed16   *new_bind;  Bind identifier of the new DSA */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              NOERROR  :  The operation was OK                      */
/*              ERROR    :  It was an error during the bind attempt   */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read-only):                                         */
/*                              num_dsa[]                             */
/* GLOBAL  DATA  (updated):                                           */
/*                              num_dsa[]                             */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/
 
 signed16 d21t047_bind_dsa(
   char         *curr_dsa,
   char         *new_dsa,
   signed16     *new_bind,
   signed16      dir_id,
   D2_c_arg     *c_arg)
 
{
   d2_ret_val  err;
   char  *file=NULL;
Bool		dummy;
D2_pbhead	dummy_pbhead; 
char norm_curr_dsa[D2_DNL_MAX + 1];
char norm_new_dsa[D2_DNL_MAX + 1];

strcpy(norm_curr_dsa, curr_dsa);
d20_norm(norm_curr_dsa, strlen(norm_curr_dsa));

strcpy(norm_new_dsa, new_dsa);
d20_norm(norm_new_dsa, strlen(norm_new_dsa));

                              /* Bind to a new DSA                    */
   if (strcmp(norm_curr_dsa, norm_new_dsa)!=0) {
      /* new_bind is not first (from logon mask) bind id.	*/
      if (*new_bind >0) {
         d20__unbind(*new_bind);
      }
                              /* bind to specific DSA                 */
      err = d20__bind((D2_name_string) new_dsa, new_bind);

      if (err < 0) {
         return(D2_ERROR);
      }
                               /* Take name of the bind DSA           */
      if (d21_bh8_get_bind_info(*new_bind, &bind_info, &dummy,&dummy_pbhead,
						FALSE) == D2_ERROR) 
      {
         d20_head->d2_retcode  = CALLERR;
         d20_head->d2_errclass = PARERR;
         d20_head->d2_errvalue = DS_NO_BIND;
         return(D2_ERROR);
      }
      strcpy(curr_dsa, (char *)bind_info.d21_b_name_dsa);
      d20_160_set_info(*new_bind, c_arg, file);
   }
                                      /* Get index of the DSA table   */
                                      /* or set the new DSA name      */
   return(D2_NOERROR);
}
 
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t048_ins_rdn()                                     */
/*                                                                    */
/* AUTHOR:      J. Pulpillo, D 525 (CDS)                              */
/* DATE:        25.10.89                                              */
/*                                                                    */
/* SYNTAX:      void d21t048_ins_rdn(first_prdn, num_rdn, n_elem,     */
/*                            object,index,pdn1,dn_no1,pdn2,dn_no2,   */
/*			      old_dn, new_dn)			      */
/*                                                                    */
/*              D20_rdn_tbl    **first_prdn;                          */
/*              D20_rdn_tbl    **num_rdn;                             */
/*              signed32        *n_elem;                              */
/*              char            *object;                              */
/*              signed32         index;                               */
/*		D20_offset     **pdn1;				      */
/*		signed32	 dn_no1;			      */
/*		D20_offset     **pdn2;				      */
/*		signed32	 dn_no2;			      */
/*		char		*old_dn;			      */
/*		char		*new_dn;			      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function inserts a new rdn into the table and    */
/*              its DSA number. After that it sorts the table.        */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              D20_rdn_tbl    **first_prdn; Ptr to the first element */
/*              D20_rdn_tbl    **num_rdn;    Current element          */
/*              signed32        *n_elem;     Number of elements       */
/*              char            *object;     Object name  	      */
/*              signed32         index;      Number of DSA            */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed32    new_bind;  Bind identifier of the new DSA */
/*              signed32    index;     Index of new DSA               */
/*                                                                    */
/* RETURNVALUE:                                                       */
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
 
 void d21t048_ins_rdn(
   D20_rdn_tbl  **first_prdn,
   D20_rdn_tbl   *first_num_rdn,
   signed32      *n_elem,
   char          *object,
   signed16       index,
   D20_offset	**pdn1,
   signed32	  dn_no1,
   D20_offset	**pdn2,
   signed32	  dn_no2,
   char		*old_dn,
   char		*new_dn)
 
{
D20_offset	**found, **pdn;
D20_rdn_tbl      par_name, *par_n[1], **p_num_rdn;
D20_rdn_tbl     *num_rdn;
char            old_name[D2_DNL_MAX+1];   /* Old DN normalized        */
signed32	dn_no;
char		*d20_beg;
 
if ((int)strlen(object) <= (int)strlen(new_dn))
	{
   	par_name.rdnn = object;
	strcpy(old_name, object);
	pdn = pdn2;
	dn_no = dn_no2;
	d20_beg = d20_beg2;
	}
else
	{
   	par_name.rdnn = object + strlen(new_dn) + 1;
      	d21t045_build_dn(new_dn, old_dn, object, old_name);
	pdn = pdn1;
	dn_no = dn_no1;
	d20_beg = d20_beg1;
	}

*par_n=&par_name;
p_num_rdn=(D20_rdn_tbl **) bsearch(par_n, first_prdn, (size_t) *n_elem,
	       (size_t) sizeof(String), 
	       (int (*)(const void *, const void *))d21t043_compare_rdn);
 
if (p_num_rdn==NULL) 
	{            /* The RDN isn't into table      */
      	num_rdn=first_num_rdn+(*n_elem); /* First free element          */
      	p_num_rdn=first_prdn+(*n_elem);
      	*p_num_rdn=num_rdn;

        found=d21t040_search_dn(pdn, dn_no, d20_beg, (Name_string) old_name);

	if ((int)strlen(object) <= (int)strlen(new_dn))
      		num_rdn->rdnn = d20_beg + (*found)->d20_mem_off;     
	else
      		num_rdn->rdnn = d20_beg + (*found)->d20_mem_off +
				strlen(new_dn) + 1;     

      	num_rdn->dsa_in[index]=1;
      	(*n_elem)++;
      	qsort(first_prdn, (size_t) *n_elem, (size_t) sizeof(String),
	    (int (*)(const void *, const void *))d21t043_compare_rdn);
   	}
else 
	{                /* The table does not contain the DSA number */
      	(*p_num_rdn)->dsa_in[index]=1;
 	}
}
 
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t049_hdl_obj()                                     */
/*                                                                    */
/* AUTHOR:      J. Pulpillo, D 525 (CDS)                              */
/* DATE:        25.10.89                                              */
/*                                                                    */
/* SYNTAX:      signed16 d21t049_hdl_obj(fd1, first_dn1, name1,dn_no1,*/
/*		      at_mem1, min_pos1, max_pos1,		      */
/*                    fd2, first_dn2, name2, dn_no2, at_mem2, 	      */
/*		      min_pos2, max_pos2, at_val_area, old_dn,        */
/*                    new_dn, object, bind_dsa)                       */
/*                                                                    */
/*              int              fd1;                                 */
/*              D20_offset     **first_dn1;                           */
/*              char            *name1;                               */
/*              signed32         dn_no1;                              */
/*		char		*at_mem1;			      */
/*		signed32	*min_pos1;			      */
/*		signed32	*max_pos1;			      */
/*              int	         fd2;                                 */
/*              D20_offset     **first_dn2;                           */
/*              char            *name2;                               */
/*              signed32         dn_no2;                              */
/*		char		*at_mem2;			      */
/*		signed32	*min_pos2;			      */
/*		signed32	*max_pos2;			      */
/*		char		*at_val_area;			      */
/*              Name_string      old_dn;                              */
/*              Name_string      new_dn;                              */
/*              Name_string      object;                              */
/*              Name_string      bind_dsa;                            */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function takes the dn offset structure of the    */
/*              object to insert and calls the add function.          */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              int              fd1;       Descriptor of the file 1  */
/*              D20_offset     **first_dn1; Ptr to the array of dn 1  */
/*              char            *name1;     Ptr to the data area      */
/*              signed32         dn_no1;    Number of objects         */
/*		char		*at_mem1;			      */
/*		signed32	*min_pos1;			      */
/*		signed32	*max_pos1;			      */
/*              int              fd2;       Descriptor of the file 1  */
/*              D20_offset     **first_dn2; Ptr to the array of dn 1  */
/*              char            *name2;     Ptr to the data area      */
/*              signed32         dn_no2;    Number of objects         */
/*		char		*at_mem2;			      */
/*		signed32	*min_pos2;			      */
/*		signed32	*max_pos2;			      */
/*		char		*at_val_area;			      */
/*              Name_string      old_dn;    Name of the old subtree   */
/*              Name_string      new_dn;    Name of the new subtree   */
/*              Name_string      object;    Object to be added        */
/*              Name_string      bind_dsa;  DSA currently binded      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
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
 
 signed16 d21t049_hdl_obj(
   int           fd1,
   D20_offset  **first_dn1,
   char         *name1,
   signed32      dn_no1,
   char         *at_mem1,
   signed32     *min_pos1,
   signed32     *max_pos1,
   int           fd2,
   D20_offset  **first_dn2,
   char         *name2,
   signed32      dn_no2,
   char         *at_mem2,
   signed32     *min_pos2,
   signed32     *max_pos2,
   char	        *at_val_area,
   Name_string   old_dn,
   Name_string   new_dn,
   Name_string   object,
   Name_string   bind_dsa)
 
{
        int             fd;
        D20_offset     *dn, **ptr;
        D20_offset    **found;
        signed32        dn_no;
        char           *n_obj, *d20_beg;
        char            n_name[D2_DNL_MAX+1];
	    char	  *at_mem;
	    signed32	  *min_pos;
	    signed32	  *max_pos;
 
                                /*  OBJECT always contain the name of */
                                /*  an existent object on the old tree*/
 
/*  The object is a parent object     */
/* it should be from new dn subtree   */
if ((int)strlen((char *)object) <= (int)strlen((char *)new_dn)) 
	{
      	d20_beg = name2;  /*  Information about the new DN      */
	at_mem = at_mem2;
	min_pos = min_pos2;
	max_pos = max_pos2;
      	ptr = first_dn2;
      	fd = fd2;
      	dn_no = dn_no2;
      	n_obj = (char *)object;
   	}
else 
	{
      	d20_beg = name1;  /*  Information about the old DN      */
	at_mem = at_mem1;
	min_pos = min_pos1;
	max_pos = max_pos1;
      	ptr = first_dn1;
      	fd = fd1;
      	dn_no = dn_no1;
      	n_obj = n_name;
      	d21t045_build_dn((char *) new_dn, (char *) old_dn,
		(char *) object, n_obj);
   	}

/*  Look for the address of the object*/
found = d21t040_search_dn(ptr,dn_no,d20_beg,(Name_string) n_obj);

dn=*found;
d20_beg1 = d20_beg;

return(d21t003_add_appnd(fd, dn, at_mem, min_pos, max_pos, OLD_ACL, 
		      (Name_string)new_dn, 
		      (Name_string)old_dn, FALSE, bind_dsa, 0, 
		      (char *)NULL, at_val_area, CHANGE_NAM));
}
 
