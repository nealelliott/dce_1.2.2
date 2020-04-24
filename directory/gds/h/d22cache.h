/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22cache.h,v $
 * Revision 1.1.10.2  1996/02/18  23:35:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:39  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:00:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:34  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:55:45  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:37:33  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:48:43  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:43:47  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:20:05  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:38:31  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:40:00  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:09  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  03:57:06  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:41:32  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D22CACHE_H
#define _D22CACHE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d22cache_rcsid[] = "@(#)$RCSfile: d22cache.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:35:35 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*                 D U A - C A C H E  (Header-file)                          */
/*                 ================================                          */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 17.11.87     */
/*****************************************************************************/

#include <dce/d27util.h>

/* ************************************************************************* */
/* ********************* DEFINITION OF CONSTANTS *************************** */
/* ************************************************************************* */

#include <sys/types.h>
#include <gds.h>

/* ------------------------------------------------------------------------- */
/* ---- default constants used within the DUA-cache configuration table ---- */
/* ------------------------------------------------------------------------- */

#define D22_CMAX_ENTRIES	 600	/* max. no. of hash table entries */
#define D22_RMAX_ENTRIES	  24	/* max. no. of RESIDENT directory objects */
#define D22_PMAX_ENTRIES	  96	/* max. no. of PRIVILEGED directory entries */
#define D22_NMAX_ENTRIES	 360	/* max. no. of NORMAL directory entries */
#define D22_IMAX_ENTRIES	  48	/* max. no. of 1st level cache directory objects */
#define D22_IOBJ_MAX		1023	/* upper limit of max. no. of 1st level cache */
					/* directory objects (depends on implementation) */
#define D22_RDEL_ENTRIES	   0	/* max. no. of removable RESIDENT directory objects */
#define D22_PDEL_ENTRIES	  19	/* max. no. of removable PRIVILEGED directory objects */
#define D22_NDEL_ENTRIES	 144	/* max. no. of removable NORMAL directory objects */
#define D22_AMAX_AREAS		  48	/* max. no. of free memory areas */
#define D22_FMAX_MEMSIZE	5000	/* max. size of all free memory areas */

/* ------------------------------------------------------------------------- */
/* ----- constants used within the DUA-cache description file (table) ------ */
/* ------------------------------------------------------------------------- */

#define D22_COPERABLE		   1	/* state of the DUA-cache */
#define D22_CINOPERABLE		  -1	/* note: this state is checked at the */
					/* activation of a DUA-cache, but never */
					/* set to D22_CINOPERABLE in this version */

					/* release of the DUA-cache files layout */
#define D22_RELEASE		"@(#)V5.0"
					/* test pattern (used to verify the machine */
					/* architecture (little/big endian)) */
#define D22_TST_PATTERN		0xAA55EEFF

/* ------------------------------------------------------------------------- */
/* ------- constants used within the DUA-cache administration table -------- */
/* ------------------------------------------------------------------------- */

#define D22_NONE		  -1	/* value of the cache-id if no DUA-cache is activated */
#define D22_RESTART		  -2	/* value of the cache-id if DUA-cache restart is requested */

/* ------------------------------------------------------------------------- */
/* --------- constants used within the 1st level cache table --------------- */
/* ------------------------------------------------------------------------- */

#define D22_IZERO		   0L	/* empty state of a 1st level cache table entry */

/* ------------------------------------------------------------------------- */
/* ------------ constants used within the DUA-hash file/table -------------- */
/* ------------------------------------------------------------------------- */

#define D22_NO_ENTRY		   0	/* empty state of an hash table entry */
#define D22_STCLMASK	      0xC000	/* storage class mask */
#define D22_RESENTRY	      0x4000	/* storage class is RESIDENT */
#define D22_PRIENTRY	      0x8000	/* storage class is PRIVILEGED */
#define D22_NORENTRY	      0xC000	/* storage class is NORMAL */
#define D22_INSTORED	      0x2000	/* 'stored in 1st level cache' flag */
#define D22_IINDEXMASK	      0x03FF	/* '1st level cache' index mask */
#define D22_TIMEBASE	       3600L	/* time base of referenced time (1 hour) */

#define D22_RESIDENT		   1	/* storage classes (constants are only used in switch-statements) */
#define D22_PRIVILEGED		   2
#define D22_NORMAL		   3

/* ------------------------------------------------------------------------- */
/* ------------ constants used within the DUA-attribute file --------------- */
/* ------------------------------------------------------------------------- */

#define D22_NAME_AFLAG	      (1 << 0)	/* naming attribute flag */
#define D22_REC_FLAG	      (1 << 1)	/* recurring attribute flag */
#define D22_RDN_FLAG	      (1 << 2)	/* relative distinguished name flag */

/* ------------------------------------------------------------------------- */
/* ------------- function return codes and operation modes ----------------- */
/* ------------------------------------------------------------------------- */

#define D22_OK			   0	/* function has been performed successfully */
#define D22_ERROR		  -1	/* function has been performed with failure */
#define D22_SPEC_ERROR		  -2	/* function has been performed with */
					/* a special failure */

#define	D22_SINGLE		   0	/* operate on one directory object only */
#define	D22_MULTIPLE		   1	/* operate on all directory objects */

/* ------------------------------------------------------------------------- */
/* ----------- constants used on directory and file operations ------------- */
/* ------------------------------------------------------------------------- */

#define D22_UMASK		   0	/* file access right mask */
#define D22_FILEMODE		0700	/* access rights of DUA-cache files */
#define D22_CDIRMODE	     0040700	/* type and access rights of DUA-cache directories */
#define D22_SEEK		   0	/* seek to given file offset */
#define D22_ESEEK		   2	/* seek to end of file */
#define D22_EOF			 -1L    /* write to end of file */

/* ------------------------------------------------------------------------- */
/* ------------- DUA-cache directory- and DUA-cache file names ------------- */
/* ------------------------------------------------------------------------- */

#define D22_CACHEDIR		"/CACHEDIR"
					/* name (dynamically extended by a specific */
					/* DUA-cache-id) of a DUA-cache directory */
#define D22_DESCFILE		"/cdescr"
					/* name of the DUA-cache description file */
#define D22_HASHFILE		"/chash"
					/* name of the DUA-cache hash file */
#define D22_ATTRFILE		"/cattr"
					/* name of the DUA-cache attribute file */
#define D22_TMPATTRFILE		"/cattr.tmp"
					/* name of the temporary DUA-cache attribute file */
					/* (used on DUA-cache reorganization) */
#define D22_REORGFILE		"/creorg"
					/* name of the DUA-cache reorganization file */
#define D22_DUMPFILE		"/cachedump"
					/* name of the DUA-cache dump file */
					/* (dynamically extended by a specific DUA-cache-id) */
#define D22_TRACEFILE		"/cachetrace"
					/* name of the DUA-cache trace file */
#define D22_LCACHEPATH		(D27_LINST_NAME + 15)
					/* max. size of the path name of the */
					/* DUA-Cache directory */

/* ------------------------------------------------------------------------- */
/* -------------- constants used for error codes --------------------------- */
/* ------------------------------------------------------------------------- */

#define D22_MINERRNO		 100	/* lowest error no. */
#define D22_MAXERRNO		 199	/* highest error no. */
#define D22_MSGSIZE_ERR		 100	/* inadmissible size of a IPC-message */
#define D22_MSYNTAX_ERR		 101	/* IPC-message syntax error */
#define D22_REQUNKNOWN_ERR	 102	/* unknown service request */
#define D22_OPTYPE_ERR		 103	/* operation error (service request is composed */
					/* by several messages) */

#define D22_VERSION_ERR		 110	/* inadmissible version no. */
#define D22_DIRID_ERR		 111	/* inadmissible directory-id */
#define D22_NOATTR_ERR		 112	/* inadmissible no. of attribute types */
#define D22_ATTSIZE_ERR		 113	/* inadmissible size of a attribute value */
#define D22_MUATTR_ERR		 114	/* attribute type several times available */
#define D22_BASEOBJ_ERR		 115	/* inadmissible operation mode on base object (DS_SEARCH) */
#define D22_NOFNDATTR_ERR	 116	/* attribute type not found */

#define D22_DNSIZE_ERR		 120	/* inadmissible size of a distinguished name */
#define D22_NMSYNTAX_ERR	 121	/* syntax error on a distinguished name */
#define D22_CHRANGE_ERR		 122	/* inadmissible character within a IA5-string */

#define D22_INOPERABLE_ERR	 130	/* inoperable DUA-cache */
#define D22_FLSIZE_ERR		 131	/* inadmissible size of a DUA-cache file */
#define D22_IBUSY		 132	/* cache data can't be accessed at the moment */

#define D22_NOEXOBJ_ERR		 140	/* directory object doesn't exist */
#define D22_HSHTABFULL_ERR	 141	/* hash table is full (can't remove any  */
					/* directory object of this storage class) */
#define D22_NOMEM_ERR		 142	/* insufficient system memory */
#define D22_INFSIZE_ERR		 143	/* wrong size of a attribute information block */

#define D22_UNKNOWNPAR_ERR	 150	/* unknown DUA-cachehandler call parameter */
#define D22_CASYNTAX_ERR	 151	/* inadmissible DUA-cachehandler call parameter syntax */
#define D22_PATHNAME_ERR	 152	/* path name exceeds limit */
#define D22_CARANGE_ERR		 153	/* DUA-cachehandler call parameter exceeds predefined range */

/* ------------------------------------------------------------------------- */
/* ------------------------ common constants ------------------------------- */
/* ------------------------------------------------------------------------- */

#define D22_RQMAXSIZE	       32766	/* max. size of a service request message */
#define D22_TRANSSIZE		1024	/* transfer buffer size for DUA-cache reorganisation */
#define D22_SRCHMAXSIZE	       16383	/* block unit size at which information is */
					/* read from the DUA-cache attribute file */
					/* on a service request DS-SEARCH */

/* ------------------------------------------------------------------------- */
/* --                         M A C R O S                                --- */
/* ------------------------------------------------------------------------- */

					/* skip IPC-message tag + tag-info */
#define D22_SKIPTAG(typ, ptr, tag, con, var)	 \
	if (*((unsigned16 *) ptr) == tag) \
		ptr = (typ *) ((byte *) ptr + con + var)

/* ************************************************************************* */
/* ******************** DEFINITION OF STRUCTURES *************************** */
/* ************************************************************************* */

/* ------------------------------------------------------------------------- */
/* -------------------- DUA-cache configuration table ---------------------- */
/* ------------------------------------------------------------------------- */

typedef struct {
	signed16	d22_hsh_max ;	    /* max. # of hash table entries */
	signed16	d22_int_max ;	    /* max. # of int. obj. entries */
	signed16	d22_res_max ;	    /* max. # of resident cache entr. */
	signed16	d22_priv_max ;	    /* max. # of privil. cache entr. */
	signed16	d22_norm_max ;	    /* max. # of normal cache entries */
	signed16	d22_rdel_no ;	    /* # of removable resident entr. */
	signed16	d22_pdel_no ;	    /* # of removable privileged entr.*/
	signed16	d22_ndel_no ;	    /* # of removable normal entries */
	signed16	d22_mem_max ;	    /* max. size (sum) of all free */
					    /* file areas */
	signed16	d22_mem_no ;	    /* max. # of free file areas */
	signed16	d22_reserved ;
	char		d22_base_path[D22_LCACHEPATH] ;
} D22_config_tab ;

/* ------------------------------------------------------------------------- */
/* ---------------- DUA-cache description file resp. table ----------------- */
/* ------------------------------------------------------------------------- */

typedef struct {
	char		d22_version[12] ;   /* cache layout version # */
	time_t		d22_cdate_time ;    /* cache creation time stamp */
	signed16	d22_cache_state ;   /* state of cache */
	unsigned32	d22_tst_pattern ;   /* architecture test pattern */
	signed16	d22_hmax_entries ;  /* max. # of hash table entries */
	signed16	d22_imax_entries ;  /* max. # of int. obj. entries */
	signed16	d22_rmax_entries ;  /* max. # of resident cache entr. */
	signed16	d22_ract_entries ;  /* act. #            "            */
	signed16	d22_rdel_entries ;  /* # of removable    "            */
	signed16	d22_pmax_entries ;  /* max. # of privil. cache entr. */
	signed16	d22_pact_entries ;  /* act. #            "           */
	signed16	d22_pdel_entries ;  /* # of removable    "           */
	signed16	d22_nmax_entries ;  /* max. # of normal cache entries */
	signed16	d22_nact_entries ;  /* act. #            "            */
	signed16	d22_ndel_entries ;  /* # of removable    "            */
	signed16	d22_fmax_memsize ;  /* max. size of free file areas */
	signed16	d22_fact_memsize ;  /* act.            "            */
	signed16	d22_amax_areas ;    /* max. # of free file areas */
	signed16	d22_aact_areas ;    /* act. #          "         */
	signed16	d22_reserved[20] ;  /* reserved */
	} D22_desc_tab ;

/* ------------------------------------------------------------------------- */
/* -------------------- DUA-cache hash entry structure --------------------- */
/* ------------------------------------------------------------------------- */

typedef struct {
	unsigned16	d22_state ;	    /* state of hash entry */
	time_t		d22_lreftime ;      /* time stamp of last reference */
	signed16	d22_obsize ;	    /* size of object (attr.) info */
	off_t		d22_oboffset ;      /* file offset of object info. */
	} D22_hash_entry ;

/* ------------------------------------------------------------------------- */
/* ------------------ DUA-cache attribute entry structures ----------------- */
/* ------------------------------------------------------------------------- */

typedef struct {
	unsigned16	d22_voff ;	    /* offset to next attribute value */
	unsigned16	d22_vlen ;	    /* size of attribute value */
	unsigned16	d22_vrep ;	    /* representation of attr. value */	
	} D22_size_info ;

typedef struct {
	signed16	d22_aetype ;	    /* attribute type */
	signed16	d22_aidsize ;	    /* size of attribute-ID */
	signed16	d22_aidsyn ;	    /* attribute syntax */
	byte		d22_aident[D2_OBJ_IDL_MAX/2] ; /* attribute-ID */
	signed16	d22_avalno ;	    /* # of attribute values */
	D22_size_info	d22_asizes ;	    /* size info. of attribute value */
	} D22_attr_entry ;	

/* ------------------------------------------------------------------------- */
/* ------------- DUA-cache 1st level cache entry structure ----------------- */
/* ------------------------------------------------------------------------- */

typedef struct {
	time_t		d22_ilreftime ;     /* last reference time stamp */
	D22_hash_entry	*d22_hshptr ;	    /* ref. to int. hash table entry */
	D22_attr_entry	*d22_iattrptr ;     /* ref. to int. attributes */
	} D22_iobj_entry ;

/* ------------------------------------------------------------------------- */
/* --------------- DUA-cache reorganisation entry structure ---------------- */
/* ------------------------------------------------------------------------- */

typedef struct {
	signed16	d22_frsize ;	    /* size of free area in attr. file*/
	off_t		d22_froffset ;      /* file offset of free area */
	} D22_fmem_area ;

/* ------------------------------------------------------------------------- */
/* -------------------- DUA-cache administration table --------------------- */
/* ------------------------------------------------------------------------- */

typedef struct {
	signed16	d22_act_cache_id ;  /* actual cache-ID */
	int		d22_desc_fd ;	    /* fd of cache description-file */
	int		d22_hsh_fd ;	    /* fd of hash-table-file */
	int		d22_attr_fd ;	    /* fd of attribute-file */
	int		d22_rorg_fd ;	    /* fd of reorganization-file */
	signed16	d22_iobjcnt ;	    /* # of 1st level cache objects */
	D22_config_tab	*d22_conftab ;	    /* ref. to internal config. table */
	D22_iobj_entry	*d22_iobjtab ;	    /* ref. to internal object table */
	D22_hash_entry	*d22_hshtab ;	    /* ref. to internal hash table */
	D22_desc_tab	*d22_desctab ;	    /* ref. to internal descr. table */
	D22_fmem_area	*d22_rorgtab ;	    /* ref. to internal reorg. table */
	} D22_admin_tab ;

/* ------------------------------------------------------------------------- */
/* -------------------- DUA-cache error translation table ------------------ */
/* ------------------------------------------------------------------------- */

typedef struct {
	signed16	d22_ival ;	/* internal error value */
	signed16	d22_ocode ;	/* error code returned */
	signed16	d22_oclass ;	/* error class returned */
	signed16	d22_oval ;	/* error value returned */
	} D22_err_trans ;

/* ------------------------------------------------------------------------- */
/* -------------- DUA-cache naming attribute reference structure ----------- */
/* ------------------------------------------------------------------------- */

typedef struct {
	D22_attr_entry	*d22_nmaptr ;
	signed16	d22_nonp ;
	signed16	d22_nmsize ;
	} D22_naming_ref ;

/* ------------------------------------------------------------------------- */
/* ---------------------------- Function prototyping  ---------------------- */
/* ------------------------------------------------------------------------- */

extern signed32 d22_attrimsg (D22_admin_tab *, byte **, D23_rsrqpb *,
		D22_attr_entry *, signed16, signed16, D2_obj_id *, D2_ret_val) ;
extern signed32 d22_activate_cache (signed16, D22_admin_tab *) ;
extern signed32 d22_cadd (D22_admin_tab *, D23_wtinpb *, D23_rsrqpb *) ;
extern signed32 d22_ccompare (D22_admin_tab *, D23_wtinpb *, D23_rsrqpb *) ;
extern signed32 d22_clist (D22_admin_tab *, D23_wtinpb *, D23_rsrqpb *) ;
extern signed32 d22_cread (D22_admin_tab *, D23_wtinpb *, D23_rsrqpb *) ;
extern signed32 d22_crmove (D22_admin_tab *, D23_wtinpb *, D23_rsrqpb *) ;
extern signed32 d22_csearch (D22_admin_tab *, D23_wtinpb *, D23_rsrqpb *) ;
extern signed32 d22_fcheck_filter (D23_ft1_info **, byte *) ;
extern signed32 d22_check_par (signed16, signed16, D23_ds_name *,
							     D22_naming_ref *) ;
extern boolean d22_cignore_comp (char *, char *) ;
extern boolean d22_scompare (D2_a_match, signed16, signed16, unsigned char *,
					   signed16, unsigned char *, boolean) ;
extern void d22_convert_to_dn (char *, D22_attr_entry *, signed16) ;
extern signed32 d22_delete_obj (signed16, D22_admin_tab *, D22_hash_entry *,
							    signed16, boolean) ;
extern void d22_hdldistcmd (int) ;
extern void d22_errorhdl (signed32, char *) ;
extern signed32 d22_eval_param (int, char *[], D22_admin_tab *) ;
extern boolean d22_evalfilter (D23_ft1_info **, D23_ft1_info *,
						   D22_attr_entry *, signed32) ;
extern signed32 d22_getfileoffset (D22_admin_tab *, signed16, signed32 *) ;
extern signed32 d22_hash_index (D2_name_string, signed32) ;
extern void d22_iinsert_obj (D22_admin_tab *, D22_hash_entry *,
							     D22_attr_entry *) ;
extern signed32 d22_insert_obj (signed16, D22_admin_tab *, D2_name_string,
						   D22_attr_entry *, signed16) ;
extern signed32 d22_mdelete_obj (signed16, D22_admin_tab *, boolean) ;
extern signed32 d22_reorg_cache (D22_admin_tab *) ;
extern signed32 d22_search_obj (D22_admin_tab *, D2_name_string,
				       D22_hash_entry **, signed16 *, boolean) ;
extern boolean d22_001_attr_comp (D2_a_value *, D2_a_value *) ;
extern void d22_terminate (int) ;
extern signed32 d22_verify_dn (D2_name_string, D22_naming_ref *) ;
extern int d22_cfopen (char *, int, int) ;
extern int d22_cfclose (int) ;
extern int d22_cfread (int, void *, size_t, off_t) ;
extern int d22_cfwrite (int, void *, size_t, off_t) ;
extern int d22_cflink (char *, char *) ;
extern int d22_cfunlink (char *) ;
extern off_t d22_cflseek (int, off_t, int) ;

#endif /* _D22CACHE_H */
