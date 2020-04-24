/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: btree.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:22  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:28:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:45  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:54  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:57:01  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:54:38  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:38:35  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:49:32  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:07  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:35:58  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:39:48  zeliff]
 * 
 * $EndLog$
 */
#ifndef _BTREE_H
#define _BTREE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char btree_rcsid[] = "@(#)$RCSfile: btree.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:34:52 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/***************************************************************************
 *
 *			   INFORMIX SOFTWARE, INC.
 *
 *			      PROPRIETARY DATA
 *
 *	THIS DOCUMENT CONTAINS TRADE SECRET DATA WHICH IS THE PROPERTY OF 
 *	INFORMIX SOFTWARE, INC.  THIS DOCUMENT IS SUBMITTED TO RECIPIENT IN
 *	CONFIDENCE.  INFORMATION CONTAINED HEREIN MAY NOT BE USED, COPIED OR 
 *	DISCLOSED IN WHOLE OR IN PART EXCEPT AS PERMITTED BY WRITTEN AGREEMENT 
 *	SIGNED BY AN OFFICER OF INFORMIX SOFTWARE, INC.
 *
 *	THIS MATERIAL IS ALSO COPYRIGHTED AS AN UNPUBLISHED WORK UNDER
 *	SECTIONS 104 AND 408 OF TITLE 17 OF THE UNITED STATES CODE. 
 *	UNAUTHORIZED USE, COPYING OR OTHER REPRODUCTION IS PROHIBITED BY LAW.
 *
 *
 *  Title:	btree.h
 *  Description:
 *		Header file for C-ISAM routines.
 *
 *  C-ISAM History:
 *	 1.00  Jan-01-82  RVH	original shipped version
 *	 1.01  Jul-13-82  RVH	fixed btadditem for deleted items
 *	 1.02  Aug-17-82  RVH	         "
 *	 1.03  Sep-21-82  RVH	fixed bad permission on audit trail
 *			 	fixed transaction/version number
 *				handle system call errors
 *				disable all locking if opened ISEXCLLOCK
 *	 1.04  May-23-83  RVH	install in UNIX Kernel
 *				some name changes to prevent conflicts
 *	 1.05  Aug-19-83  RVH	lots of speed enhancements
 *				fast sequential reads available
 *	 2.00  Sep-01-83  RVH	shipment version number
 *	 2.01  Oct-18-83  RVH	check for deleting current key
 *				update problem with treelev
 *				piped ISAM additions
 *	 2.02  Jan-06-84  RVH	allocate checks
 *				final kernel installation
 *				RDSLOCK schem for kernel installation
 *				concurrent isreads (possible hogging of file)
 *				   for RDSLOCK and ONIXLOCK
 *				save records locked so rewrite/delete may not
 *				   have to reread record.
 *				new externals: isrecnum, issingleuser
 *				custom collating
 *	 2.03  Apr-16-84  RVH	released version number
 *	 2.10  Oct-30-84  RVH	allow complete record number interface
 *				isdelrec, isrewrec, read by recnum
 *
 ************************************************************************
 */


#define ISVOPEN 1

#define COPYRT	"Copyright (C) 1981-1988 Informix Software, Inc."

#include <machine.h>
#include <isam.h>

#ifdef SEMLOCK
#define CREATLOCK
#endif /* SEMLOCK */

#ifdef	PCDOS
#define	open	pcopen
#endif	/* PCDOS */

#ifdef LATTICE			/* define PCDOS types	*/
typedef short dev_t;
typedef short ino_t;
/*for PCDOS, use rds functions rather than lattice functions*/
#define creat pccreat
#define read  pcread
#define write pcwrite
#define lseek pclseek
#define close pcclose
#define unlink pcunlink
#define rename pcrename

#include <errno.h>
#else				/* define UNIX types	*/
#ifndef ISKERNEL
#include <errno.h>

#ifndef VMS
#include <sys/types.h>
#include <sys/stat.h>
#else /* VMS */
#include <types.h>
#define ino_t vms_ino_t
#define dev_t vms_dev_t
#include <stat.h>
#undef ino_t
#undef dev_t
typedef struct {unsigned short ino_t_array[3];} ino_t;
typedef struct {char dev_t_array[64];} dev_t;
#include <perror.h>
#endif /* VMS */

#ifdef  SEMLOCK
#include <sys/ipc.h>
#include <sys/sem.h>
#endif  /* SEMLOCK */
#else				/* define UNIX kernel types */
#define RDSLOCK
#include <sys/param.h>
#endif  /* ISKERNEL */
#endif  /* LATTICE */

#ifndef ISKERNEL
#define sysmemory()
#define usrmemory()
#define fubyte(p)	(*(p) & BYTEMASK)
#define subyte(p,b)	(*(p) = (b))
#endif  /* ISKERNEL */

#ifndef STATIC
#define STATIC
#endif

#ifdef DRVRLOCK
#include <sys/lock.h>
#endif

#ifdef NOVALIDATE
#define	snverify(a,b,c,d)
#define	snchk()
#endif /* NOVALIDATE */
#include <sn.h>

#ifdef lint		/* keep lint from complaining */
#define VOID	_VOID_ = (int)
#else
#define VOID
#endif

#ifdef  NULL
#undef  NULL
#endif
#define NULL	    0
#define OPENERR     -1

#ifdef  SMALLSYS
#define	NBUFS	    8
#endif

#ifdef  ISKERNEL
#define NFILES	    40		/* max number of isam files	*/
#define NOPENS	    80		/* max number of opens allowed	*/
#define NBUFS	    32		/* number of btree node buffers	*/
#else
#define NFILES	    64		/* max number of isam files	*/
#define NOPENS	    64		/* max number of opens allowed	*/
#ifndef NBUFS
#define NBUFS	    16		/* number of btree node buffers	*/
#endif  /* NBUFS */
#endif  /* ISKERNEL */

#define NLOCKS	   64		/* max number of record locks	*/
#ifdef VMS
#define LPADSIZE   NODESIZE - 388 /* pad for create lock        */
#endif /* VMS */
#define NTREES	   10		/* max b-tree level		*/
#define KEYSIZE	   120		/* max length of a key		*/

#ifndef	NODESIZE
#define NODESIZE   512		/* btree node size		*/
#endif	/* NODESIZE */

#ifndef VMS
#ifndef SINIX
#define PATHSIZE   128		/* file pathname length		*/
#else /* SINIX, TP 13.06.88 */
#define PATHSIZE   256          /* file pathname length         */
#endif /* SINIX */
#else /* VMS */
#define PATHSIZE   256          /* file pathname length         */
#endif /* VMS */
#define ISMAGIC    0177123	/* ISAM file magic number	*/

#define RMODE	   0		/* file open modes	*/
#define WMODE	   1
#define RWMODE	   2

#define PROFILE(p)		/* no profiling		*/

#define UNLOCK     0		/* file lock modes	*/
#define LOCKUNCOND 1
#define LOCKCOND   2
#define LOCKTEST   3
#define LOCKSHARE  4		/* shared lock mode bit	*/
#define ISREAD     0x08		/* mode bit to isenter from isread	*/
#define ISUNLOCK   0x10		/* mode bit to isenter from isrelease	*/
#define INDXREC	   0		/* allocate index record	*/
#define DATAREC    1		/* allocate data record		*/

#define ERROR (-1)
#define OLD_AUTO   0x0300	/* old value for ISAUTOLOCK */
#define OLD_EXCL   0x0500	/* old value for ISEXCLLOCK */
#define LOCKMASK   0xFF00	/* mask for lock flags	    */
#define IOMASK	   0x00FF	/* mask for I/O read flags  */

#ifdef	NOCASTS			/* for C-compiler without casts	*/
#define CASTCP
#define CASTIP
#define CASTIN
#define CASTUN
#define CASTSP
#else
#define CASTCP (char *)		/* char pointer cast		*/
#define CASTIP (int *)		/* int pointer cast		*/
#define CASTIN (int)		/* int cast			*/
#define CASTUN (unsigned)	/* unsigned int cast		*/
#define CASTSP (short *)	/* short pointer cast		*/
#endif

#define DATNAME ".dat"		/* extension for data file	*/
#define DATMODE  0664
#define IDXNAME ".idx"		/* extension for index file	*/
#define IDXMODE  0664
#define LOKNAME ".lok"		/* extension for lock file	*/
#define LOKMODE  0666

#define SEMNAME "/isam/lock"	/* prefix for semaphore file	*/
#define SEMMODE  0444

#ifdef  SEMLOCK			/* system V semaphore defines	*/
#define NUMSEMS		20
#define SEMKEY		(key_t)0xDB000000
#define SEMHASH(d,i)	(int)((d+i)%NUMSEMS)
#endif  /* SEMLOCK */

#define ld1(p)		((p)[0]&BYTEMASK)
#define st1(c,p)	((p)[0]=(c))

#ifdef MPM86
#undef  ldint
#undef  stint
#endif

#define ld2(p)		ldint(p)
#define st2(i,p)	stint(i,p)
#define ld4(p)		ldlong(p)
#define st4(l,p)	stlong(l,p)

#ifndef NOLONGMULT
#define longmult(x,y)	((x)*(y))
#else
long longmult();
#endif

struct item
    {
    short it_flags;		/* flag bits			*/
    short it_totlen;		/* total length			*/
    short it_keylen;		/* key length			*/
    unsigned it_dupnum;		/* duplicate number		*/
    long it_ptr;		/* pointer			*/
    short it_leadcnt;		/* leading count		*/
    short it_trailcnt;		/* trailing blanks count	*/
    char it_key[KEYSIZE];	/* key value			*/
    };
#define DUP	01
#define NEXTDUP	02

struct dict
    {
    char d_magic[2];		/* magic number = BTMAGIC	*/
    char d_dummy1[4];		/* reserved bytes = 2,2,4,4	*/
    char d_idxsize[2];		/* size of index record		*/
    char d_nkeys[2];		/* number of keys defined	*/
    char d_cflag[1];		/* compression flag bits	*/
    char d_dflag[1];		/* duplicates flag bits		*/
    char d_version[1];		/* file version number		*/
    char d_recsize[2];		/* data record size		*/
    char d_keysnode[4];		/* key's description node	*/
    char d_dummy2[6];
    char d_freedata[4];		/* free data file record list	*/
    char d_freenode[4];		/* free b-tree node list	*/
    char d_nextdata[4];		/* next data file record	*/
    char d_nextnode[4];		/* next b-tree node		*/
    char d_transaction[4];	/* transaction number		*/
    char d_uniqid[4];		/* next available unique id	*/
    char d_audit[4];		/* audit trail info		*/
#ifdef VMS
    char d_pad[NODESIZE - 53];  /* pad dictionary to NODESIZE   */
#endif /* VMS */
    };

struct node
    {
    char n_used[2];		/* num bytes used in this node	*/
    char n_items[507];		/* items with greater pointer	*/
    char n_index;		/* index number			*/
    char n_level;		/* 0 = leaf, 1..N = internal	*/
    };

struct free
    {
    char fn_used[2];		/* num bytes of free entries	*/
    char fn_next[4];		/* ptr to next free list node	*/
    char fn_free[126] [4];	/* free entries			*/
    char fn_kind;		/* FE = free nodes, FF = data	*/
    char fn_type;		/* = 7F				*/
    };

struct keyn
    {
    char kn_used[2];		/* num bytes used		*/
    char kn_next[4];		/* ptr to next key desc node	*/
    char kn_keys[503];		/* key descriptions		*/
    char kn_flag;		/* = FF				*/
    char kn_type;		/* = 7E				*/
    };

struct audt
    {
    char an_used[2];		/* num bytes used		*/
    char an_flags[2];		/* audit flags			*/
    char an_filename[PATHSIZE];	/* audit trail file name	*/
    char an_type;		/* = 7D				*/
    };

#ifdef ZCC			/* for ONIX zcc C-compiler	*/
struct ifile {};
#endif

#ifndef SHMBUFF
#ifdef ISBUFHASH
struct hbuf
    {
    short bf_flags;
    struct buff *bh_forw;
    struct buff *bh_back;
    };
#endif /* ISBUFHASH */

struct buff			/* V104 */
    {
    short bf_flags;		/* flags			*/
#ifdef ISBUFHASH
    struct buff *bh_forw;
    struct buff *bh_back;
#endif /* ISBUFHASH */
    struct buff *bf_forw;	/* forward pointer		*/
    struct buff *bf_back;	/* backward pointer		*/
#ifdef ISKERNEL			/* V104 */
    struct buf *bf_bp;		/* kernel buffer pointer	*/
#else
    struct ifile *bf_filep;	/* file pointer			*/
#endif
    long  bf_daddr;		/* disk address			*/
    union
	{
	struct dict *bf_dict;	/* ptr to dictionary		*/
	struct keyn *bf_keyn;	/* ptr to key desc node		*/
	struct node *bf_node;	/* ptr to btree node		*/
	struct free *bf_free;	/* ptr to free list		*/
	struct audt *bf_audt;	/* ptr to audit trail info	*/
	char *bf_addr;		/* pointer to raw data		*/
	int  *bf_words;		/* int pointer to raw data	*/
	} bf_un;
    short bf_used;		/* # bytes used in node		*/
    char bf_type;		/* type byte in node		*/
    char bf_level;		/* b-tree level			*/
    };
#define BFMODF 001		/* buffer modified		*/
#define BFDONE 002		/* buffer contains data		*/
#define BFBUSY 004		/* buffer in use		*/
#define BFKBUF 010		/* kernel buffer allocated	*/

#define DATABUF 0x80000000	/* flag for bfread(data buffer)	*/
#endif  /* SHMBUFF */

struct tree
    {
    long t_node;		/* btree node number		*/
    short t_offset;		/* offset into node		*/
    short t_nextoff;		/* offset of next item in node	*/
    short t_used;		/* number bytes used in node	*/
    };
#define BEGFILE 01		/* at beginning of file		*/
#define ENDFILE 02		/* at end of file		*/

struct reclist			/* list of locked records	*/
    {
    struct reclist *rl_next;	/* next record			*/
    long rl_recnum;		/* record number		*/
    };

#ifndef ONIXLOCK
#ifndef DRVRLOCK
struct lock
    {
    short l_nopens;		/* num open locks, -1 = excl	*/
    short l_filelock;		/* file lock flag		*/
    short l_procid[NLOCKS];	/* process ids, 0 = no lock	*/
    long l_recnum[NLOCKS];	/* record numbers		*/
#ifdef VMS
    char l_pad[LPADSIZE];       /* pad to NODESIZE              */
#endif /* VMS */
    };
#endif /* DRVRLOCK */
#endif /* ONIXLOCK */

struct ifile
    {
    short f_flags;
    short f_ucount;		/* number of opens		*/
    short f_xlkcnt;		/* number of exclusive opens	*/
    short f_flkcnt;		/* number of file locks		*/
    struct reclist *f_wlock;	/* list of rec write locks	*/
    struct reclist *f_rlock;	/* list of rec read locks	*/
    dev_t f_dev;		/* V202 device number of file	*/
    ino_t f_inum;		/* V202 inode number of file	*/
#ifdef VMS
    ino_t f_datinum;		/* Data file id for VMS		*/
    ino_t f_st_ino;		/* Directory file id for VMS	*/
#endif /* VMS */
#ifdef SEMLOCK
    short f_semid;		/* SYSTEM V semaphore ident	*/
#endif /* SEMLOCK */
    long f_version;		/* transaction version number	*/
#ifdef ISKERNEL			/* V104 */
    struct inode *f_datfd;	/* data inode ptr		*/
    struct inode *f_idxfd;	/* index inode ptr		*/
    struct inode *f_audfd;	/* audit inode ptr		*/
#else
    short f_datfd;		/* data file fd			*/
    short f_idxfd;		/* index file fd		*/
    short f_audfd;		/* audit file fd		*/
#endif
#ifdef CREATLOCK
    short f_lokfd;		/* lock file fd			*/
#endif
    short f_datsize;		/* data record size		*/
    short f_idxsize;		/* index record size		*/
#ifdef SHMLOCK
    struct smfile *f_smfile;	/* ptr to common file info	*/
#else  /* SHMLOCK */
    struct dict f_dict;		/* isam dictionary header	*/
#endif /* SHMLOCK */
#ifdef ISVOPEN
     char f_fname[PATHSIZE];	/* name of file			*/
     short f_omode;		/* open mode			*/
     short f_vrefcnt;		/* reference cnt for vopens	*/
#endif /* ISVOPEN */
#ifdef MEMLOCK
    struct lock f_locks;	/* in memory lock table		*/
#define locks	filep->f_locks
#endif
    };
#define FMODF	 01		/* file has been modified	*/
#define FDICTMOD 02		/* dictionary modified		*/
#define FAUDOPEN 04		/* audit trail is open		*/
#define FLOCKS  010		/* have some data record locks	*/

struct iopen
    {
    short o_flags;		/* status flags			*/
    short o_procid;		/* process id number		*/
    short o_userid;		/* user id number		*/
    short o_mode;		/* open file mode		*/
    long  o_version;		/* transaction version number	*/
#ifdef ISKERNEL
    struct kobuf *o_kopenp;	/* cross-link for kernel buffer	*/
#endif /* ISKERNEL */
    struct ifile *o_filep;	/* file descriptor pointer	*/
    struct keydesc o_curkey;	/* current key descriptor	*/
    struct item o_curitem;	/* current item value		*/
    struct tree o_trees[NTREES];/* current tree path		*/
    struct reclist *o_tmpfree;	/* temporary free record list	*/
    short o_trlev;		/* current tree level		*/
    short o_trleaf;		/* tree level of leaf node	*/
    short o_trflags;		/* tree flags			*/
    };
#define OBUSY	 0001		/* open struct in use		*/
#define OCURPOS	 0004		/* current position exists	*/
#define OCURREAD 0010		/* current record has been read	*/
#define ODUPL	 0020		/* duplicate created or read	*/
#define OFLOCK	 0040		/* have whole file locked	*/
#define OSLOCK   0100		/* have semaphore lock		*/
#define OFLUSH   0200		/* write out buffers to disk	*/
#define ORLOCK   0400		/* have some record locks	*/

#ifdef SHMLOCK
#ifndef VMS
#include <shmisam.h>
#else /* VMS */
#include <gsisam.h>
#endif /* VMS */
#endif /* SHMLOCK */

struct buff *bfread();
struct buff *bfgetblk();
struct buff *bfgetclr();

long lseek();
long allocrec();
struct ifile *allocfile();
#ifdef ISALLOC
char *malloc();
#endif /* ISALLOC */


/* defines for transaction */

struct filelist
    {
    short fl_oisfd;				/* original open isfd */
    short fl_risfd;				/* recovery open isfd */
    short fl_mode;				/* open mode */
    short fl_flag;				/* flags - see below */
    char *fl_path;				/* dir path for file */
    struct filelist *fl_next;			/* next in list */
    };

/* define for fl_flag
 */
#define FL_ERASED	001			/* file erased in tx */
#define FL_CLOSED	002			/* file logically closed */


#define ISDELETE	0			/* log deleted record */
#define ISINSERT	1			/* log inserted record */
#define ISUPDATE	2			/* log updated record */

#define ISFOPEN		0			/* log open file */
#define ISFCLOSE	1			/* log close file */

#define ISCINDEX	0			/* log create index */
#define ISDINDEX	1			/* log drop index */
#define ISCLUSIDX       2			/* log create cluster idx */

#define TRANS		0			/* has transactions */
#define NOTRANS		1			/* no transactions */
#define STRANS		2			/* singleton transaction */

#ifdef VMS       /*  VMS specific i/o functions */
#define open qioopen
#define close qioclose
#define lseek qiolseek
#define creat qiocreat
#define write qiowrite
#define read qioread
	/* VMS specific defines for executive mode linkage to isam */
#define isbuild vms_isbuild
#define isaddindex vms_isaddindex
#define isdelindex vms_isdelindex
#define iserase vms_iserase
#define isrename vms_isrename
#define isopen vms_isopen
#define isclose vms_isclose
#define isrelease vms_isrelease
#define issetsu vms_issetsu
#define islock vms_islock
#define isunlock vms_isunlock
#define isuniqueid vms_isuniqueid
#define isindexinfo vms_isindexinfo
#define islogopen vms_islogopen
#define islogclose vms_islogclose
#define issusplog vms_issusplog
#define isresumlog vms_isresumlog
#define isbegin vms_isbegin
#define iscommit vms_iscommit
#define isrollback vms_isrollback
#define isrecover vms_isrecover
#define isaudit vms_isaudit
#define isread vms_isread
#define isstart vms_isstart
#define iswrite vms_iswrite
#define iswrcurr vms_iswrcurr
#define isrewrite vms_isrewrite
#define isrewcurr vms_isrewcurr
#define isdelete vms_isdelete
#define isdelcurr vms_isdelcurr
#define isremlock vms_isremlock
#define isversion vms_isversion
#define issetunique vms_issetunique
#define iscleanup vms_iscleanup
#define isrewrec vms_isrewrec
#define isdelrec vms_isdelrec
#define iststindex vms_iststindex
#endif /* VMS */

#ifdef SINIX
#define kycopy(s,d)	(*(d) = *(s))

#define itcopy(s,d,k)	bycopy((char *)(s),(char *)(d),((sizeof(struct item))-KEYSIZE)+(k)->k_len)

#endif /* SINIX */

#endif /* _BTREE_H */
