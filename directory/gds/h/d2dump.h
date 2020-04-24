/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2dump.h,v $
 * Revision 1.1.10.2  1996/02/18  23:36:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:23  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:01:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:11  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:18:34  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:30:58  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:52:37  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:47:26  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:51:11  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:42:47  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:41:45  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:30:17  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  04:09:41  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:43:09  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D2DUMP_H
#define _D2DUMP_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d2dump_rcsid[] = "@(#)$RCSfile: d2dump.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:36:03 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton****************************************************************/
/*								      */
/* TYPE         : INCLUDE                                             */
/*								      */
/* NAME         : d2dump.h        structures for dump/restore         */
/*								      */
/* AUTHOR       : Volpers                                             */
/* DATE         : 26.04.1988                                          */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOK.-NR.     : DS-Design spezification                             */
/*								      */
/* PRD#/VERS.	:						      */
/*								      */
/* DESCRIPTION  : contains all typedefs of data structure for dumping */
/*                and restoring subtrees                              */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 20.08.87| Original                       |    |         */
/*datoff***************************************************************/

#define D20_HDLD       0        /* a flag indicating whether the DN   */
				/* (or attribute in attribute list)   */
				/* was already handled                */

#define D20_0_SEARCH      0     /* operations for IAPL Functions      */
#define D20_1_ADD_OBJ     1
#define D20_2_DEL_OBJ     2
#define D20_3_MOD         3
#define D20_5_ALIAS       5

#define D20_FILE_MAX    100     /* max. size of test file name        */

#define D20__BEGIN      0x01
#define D20__CONTINUE   0x02
#define D20__END        0x04

#define D20_SE1         "SEARCH1"    /* filename for dump and restore */
#define D20_SE2         "SEARCH2"    /* filename for dump and restore */


				    /* ERROR VALUES                   */
#define D20_FILE_ERR     -123
#define D20_NOFILE       -124       /* internal dump file not found   */
#define D20_SEARCH_ERR   -200
#define D20_ADD_ERR      -201
#define D20_MOD_ERR      -202
#define D20_DEL_ERR      -203
#define D20_ALLOC_ERR    -1001


				    /* state of DNs in internal DN area */
#define D20_DONE            0x01    /* DN already handled               */
#define D20_INSTALL         0x02    /* DN (copy) has to be installed    */
#define D20_DONT_INSTALL    0x04    /* DN must not be installed in the  */
				    /* shadow DSA                       */

				   /* used in the append subtree function */
#define D21_PARENT_OBJ     0x008   /* Object over the subtree         */
#define D21_REAL_OBJ       0x010   /* Object belonging to the subtree */
#define D21_EXTERN_OBJ     0x020   /* Object outside of the subtree   */
				   /* and his parents                 */
#define D21_ALIAS_OBJ      0x040   /* object is an alias object       */

#define D20_NOVAL   8               /* number of "longs" to be read   */
				    /* from internal dump file        */

#define D20_DN_NO       300     /* number of expected DNs             */

#define D20_INT_DN_NO    50     /* number of internal DNs (aliased    */
				/* objects and DSA-names)             */

#define D20_DN_SIZE     (64 * BUFSIZ)
				/* memory area for DNs                */

#define D20_MORE        (10 * BUFSIZ)
				/* length of memory area that will    */
				/* additonally be used after          */
				/* reallocation (for DNs)             */

#define D20_NO_OFF     100      /* number of offset blocks that will  */
				/* additionally be used after         */
				/* reallocation                       */

#define D20_LOG_INHOVR	30	/* max. size of log-string for inherit*/
				/* and overwrite		      */

#define D20_LOG_SUBSET	15	/* max. size of log-string for subset */


typedef struct {
            char dsan[D2_DNL_MAX+4];    /* Name of DSA                */
            signed32  ind;              /* Index of the DSA           */
        } D20_dsa_tbl;
 
 
typedef struct {
            char *rdnn;    		/* RDN of the object relative */
					/* to new parent.	      */
            char dsa_in[D20_INT_DN_NO]; /* DSAs in which it exists    */
        } D20_rdn_tbl;
 

typedef struct
	    {   signed32  d20_file_off;     /* offset of DN in file       */
		signed32  d20_mem_off;      /* offset of DN in memory area*/
		signed32  d20_attr_len;     /* length of attribute list   */
		signed32  d20_state;        /* state of DN in internal    */
					/* memory area                */
	    } D20_offset;


typedef struct
	    {   char      *names;       /* memory area for DNs        */
		signed32   anz;         /* actual length of DNs       */
		signed32   max_anz;     /* max. length of DNs         */
	    } D20_dns;


typedef struct
	    {   D20_offset  **poff;     /* pointer array of offset    */
					/* structures                 */
		D20_offset   *off;      /* offsets of DNs             */
		signed32      no;       /* actual number of DNs       */
		signed32      max_no;   /* max. number of DNs         */
	    } D20_dn_off;


typedef struct
	    {   Bool    d20_sobj;       /* indicates whether object   */
					/* name is to be interpretated*/
					/* as single object or not    */
		char    d20_obj[D2_DNL_MAX+4];  /* object name        */
		char    d20_dsa[D2_DNL_MAX+4];  /* DSA name of master */
						/* (== DSA that propa-*/
						/* gates shadows)     */
		signed16 d20_dir_id;     /* directory identifier       */
		signed32 d20_time_stamp; /* time stamp of dumping      */
	    } D20_delupd_info;


typedef struct
	{  D2_pbhead         *d20_Bpbhead;    /* -> pointer to head   */
	   signed16           d20_B1bind_id;  /* -> bind_id to dsa    */
					      /* which contains the   */
					      /* subtree              */
	   IA5_string         d20_Bo_file;    /* -> name of the file  */
					      /* which contains the   */
					      /* subtree              */
	   D2_name_string     d20_Bsubtree;   /* -> pointer to subtree*/
	   Bool               d20_Bsingle_ob; /* single object if TRUE*/
					      /* subtree if FALSE     */
	   D2_c_arg          *d20_B1c_a;      /* -> pointer to the    */
					      /* common arguments     */
					      /* (for dump)           */
	   signed16           d20_B2bind_id;  /* -> bind_id to dsa    */
					      /* which takes up the   */
					      /* subtree              */
	   IA5_string         d20_Bn_file;    /* -> name of the file  */
					      /* which takes up the   */
					      /* subtree              */
	   D2_c_arg          *d20_B2c_a;      /* -> pointer to the    */
					      /* common arguments     */
					      /* (for restore)        */
	   D2_name_string     d20_Bdsa;       /* -> name of master DSA*/
					      /*    of objects that   */
					      /*    be propagated via */
					      /*    "delta_updates()" */
	   signed16           d20_Bdir_id;    /* -> directory id      */
	   signed32           d20_Bstime;     /* <- start time of     */
					      /*    dumping           */
	   signed32           d20_Betime;     /* <- termination time  */
					      /*    of dumping        */
	} D20_pB;

#define D20_BUFSIZ     4096

#endif /* _D2DUMP_H */
