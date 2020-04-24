/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cxioctl.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:43  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:02  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:49:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:56  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:56  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:04  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:54  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:23  marrek]
 * 
 * $EndLog$
 */
#ifndef _CXIOCTL_H
#define _CXIOCTL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cxioctl_rcsid[] = "@(#)$RCSfile: cxioctl.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:43 $";
#endif

/* #ident "@(#)cxioctl.h	311.11 93/08/24 CMX (NC1)" */

/*
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1992
 *               All Rights Reserved
 *
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice.   This software may not
 * be provided or otherwise made available to, or used by, any
 * other person.  No title to or ownership of the software is
 * hereby transferred.
 */
/*
 * ioctl constants and structure of ICMX(S3)
 *
 * (see also c*ioctl.h for the CC adapter
 *  and cxtioctl.h for the CMX/XTI automaton)
 *
 * General format of system call:
 *
 * ioctl(fd, cmd, parg)
 *	int	fd;	file descriptor
 *      int     cmd;    command id
 *      ...     *parg;  pointer to argument
 *
 * User status:
 * RK	(NC152)	02.07.91	Basic version CMX V3.1 (<- 3.7).
 * RK	(NC152)	27.08.91	Definitions for automatic address generation.
 * RK	(NC152)	17.01.92	CX_AADMIN abd CX_ATEST minor numbers changed.
 * RK	(NC152)	03.02.92	CC identifiers substitute CC bits.
 * RK	(NC152)	27.05.92	Own field for up_mignea.
 * RK	(NC152)	27.07.92	Enhancements for XPG4 XTI added.
 * LA   (NC152) 25.08.92        RM700I/RM700R version IDs added
 * RK	(NC152)	17.09.92	Enhanced for XPG4 XTI option management.
 * RK	(NC152)	22.10.92	Pseudo adaptor IDs added.
 */

/*
 * Special minor device numbers
 */
#define CX_AADMIN       4094    /* Device file for CMX/XTI administration */
#define CX_ATEST        4095    /* Device file for diag. of CMX/XTI autom. */

/*
 * Maximum number of CCs that can be used by CMX/XTI automaton
 */
#define CX_NCC		16

#ifndef CX_NCCINDEX                     /* If not defined in cmxss2.h */
#define CX_NCCINDEX	16
#endif

/*
 * General applicable time entry structure, in
 * accordance with struct timeval (time.h)
 */
struct cx_time
{
	long            cxt_sec;        /* Seconds since 1.1.1970, 0 hours */
	long            cxt_usec;       /* additional microseconds */
};

/*
 * Time stamp structure (for library etc.)
 * For ioctl CX_TIMESTAMP
 */
struct cx_timestamp
{
	long            ts_sec;         /* Seconds since 1.1.1970, 0 hours */
	short           ts_msec;        /* additional milliseconds */
};

/*
 * Data structure to store a CC list.
 */
struct cc_list_t
{
	unsigned short	cl_num;		/* number of elements in cl_id, or: */
#define	CX_ALLCCIDS	0xffff
	unsigned short	cl_id[CX_NCC];	/* list of CC identifiers */
};

/*
 * command ID macro CX_IO
 *
 * For SINIX V5.[0-2]*, must correspond with _IO from ioctl.h;
 * _IO is generally not appliable due to problem in SCO compiler
 */
#define CX_IOC_VOID	0x20000000
#define CX_IO(cmdchar,n) (CX_IOC_VOID | (cmdchar) << 8 | (n))

/*
 * IDs of help commands for the CMX- (as of V2.5) and XTI libraries
 * and for the CMX/XTI daemon
 * at CMX/XTI automaton (administration device file);
 * structure of ioctl argument (*parg) depends on command (see above);
 * Response 0, if error free
 */
#define CX_PRENETOPEN   CX_IO('C',8)    /* Check compatibility of
					   library and CMX/XTI automaton and
					   request number of a free network
					   device file;
					   the argument has the structure
					   cx_prenetopen (see above);
					   in cxo_version, the relevant
					   library function specifies the
					   version of the library before the call
					   (see "Versions specification" at end of
					   cxioctl.h) and then specify its own
					   version with no computer type, if
					   it is not greater than that of the
					   library (otherwise it returns the
					   library version) and it can work
					   with the library (otherwise error);
					   in cxo_mindev, the automaton
					   specifies the number of a free
					   network device file (binary) */

/*
 * Argument structure for command CX_PRENETOPEN
 */
struct cx_prenetopen
{
	unsigned short  cxo_version;    /* Version ID, see CX_PRENETOPEN */
	unsigned short  cxo_mindev;     /* Number of free network device file
					   (minor device number) */
};

/*
 * Returns an exact time stamp (struct cx_timestamp)
 * to the library
 */
#define CX_TIMESTAMP    CX_IO('x',0x16) /* time stamp (for library) */

/*
 * IDs of CMX- and XTI commands for the CMX- (as of V2.5) and XTI-
 * libraries to CMX/XTI automaton (network device files);
 * structure of ioctl arguments (*parg) is struct cx_upar_t (see above),
 * their (command dependent) application is described in the ICMX(S3)
 * Specification Part B;
 * the returns are also described there
 *
 * Each of these IDs contain a number (n), a length specification (l)
 * and an ID letter, as defined in the following two macros,
 * and if necessary, one or more flags (CX_FKERNARG, CX_FKERNDAT, CX_FKERNETC).
 * The number are defined below.
 * The length specifies the number of argument bytes from argument start the
 * automaton must copy down; it knows itself the number of bytes it can
 * copy over. The length depends on commands and lies between 0 (i.e. nothing
 * to copy) and sizeof(struct cx_upar_t).
 */
#define ST_SCMD(n,l)	(CX_IO('S',n)|((l)&0xff)<<16) /* baut CMX-K.kennung */
#define XT_SCMD(n,l)	(CX_IO('X',n)|((l)&0xff)<<16) /* baut XTI-K.kennung */
#define CX_FKERNARG     0x80               /* Flag in command ID, if the argument
					      is not to be copied from/to the
					      user space, rather to the kern address
					      address space */
#define CX_FKERNDAT     0x40               /* Flag in command ID, if the user data
					      is not to be copied from/to the
					      user space, rather to the kern
					      address space */
#define CX_FKERNETC     0x20               /* Flag in command ID if other (local
					      name, T.addresses, TSP options,
					      name of T.service) is not to be copied
					      from/to the user space, rather to the
					      kern address space */
/*
 * Using the following macros, the CMX/XTI automaton evaluates the following IDs:
 */
#define CX_GCMDCHAR(cmd) (((cmd)>>8)&0xff) /* specifies ID letter of c. */
#define ST_S2           's'                /* ID letter of CMX commands
					      of ICMX(S3) Part A = ICMX(S2) */
#define ST_S3B          'S'                /* ID letter of CMX commands
					      of ICMX(S3) Part B */
#define XT_S3B          'X'                /* ID letter of XTI commands
					      of ICMX(S3) Part B */
#define CX_GCMDID(cmd)  ((cmd)&0x1f)       /* specifies number of command */
#define CX_GCMDL(cmd)   (((cmd)>>16)&0xff) /* specifies length (see above) */
#define CX_GCMDFLAGS(cmd) ((cmd)&(CX_FKERNARG | CX_FKERNDAT | CX_FKERNETC))
					   /* specifies command flag (see above.) */

/*
 * Numbers of CMX commands
 *
 * Note: Values must correspond to last bytes of corresponding command IDs
 * of ICMX(S2) (see cmxss2.h) !
 */
#define STNOCMD         0               /* invalid */
#define STINIRQ         1               /* logon application for process */
#define STTRMRQ         2               /* log off application for process */
#define STEVENT         3               /* request transport event closest to
					   process, but do not wait
					   for it */
#define STCONRQ         4               /* request connection set up */
#define STCONIN         5               /* fetch connection setup display */
#define STCONRS         6               /* accept connection setup */
#define STCONCF         7               /* fetch connection setup confirmation */
#define STDISRQ         8               /* request connection cleardown */
#define STDISIN         9               /* fetch connection cleardown display */
#define STREDRQ         10              /* request connection redirection */
#define STREDIN         11              /* fetch connection redirection */
#define STDATRQ         12              /* send TIDU on connection */
#define STDATIN         13              /* fetch TIDU on connection */
#define STXDTRQ         14              /* send ETSDU on connection */
#define STXDTIN         15              /* fetch ETSDU on connection */
#define STDATSTOP       16              /* quit, so that the normal connection
					   data is no longer displayed with ST*EVENT */
#define STXDTSTOP       17              /* so that normal and priority connection data
					   is no longer displayed with ST*EVENT */
#define STDATGO         18              /* quit, so that the normal and priority data
					   of a connection are displayed again
					   with ST*EVENT */
#define STXDTGO         19              /* quit, so that the priority connection data
					   to be fetched with ST*EVENT is displayed
					   again */
#define STINFO          20              /* fetch information on connection transport
					   service provider */
#define ST2INFO         21              /* fetch information on transport service
					   provider (not for a particular
					   connection) */
#define STVEVENT        22              /* as STEVENT, but wait restricted
					   until a transport event to be
					   displayed is available */
#define STWEVENT        23              /* as STEVENT, but unrestricted wait
					   if necessary, until transport event
					   to be displayed is available */
#define ST_NCMD         24              /* Number of commands */

/*
 * Numbers of XTI commands
 */
#define XTNOCMD         0               /* invalid */
#define XTBIND          1               /* logon application for TEP */
#define XTUNBIND        2               /* log off application for TEP */
#define XTLOOK          3               /* request relevant transport event
					   closest to TEP */
#define XTCONNECT       4               /* request connection setup;
					   if necessary, wait for confirmation
					   and fetch it */
#define XTLISTEN        5               /* fetch connection setup display;
					   wait if necessary  */
#define XTACCEPT        6               /* accept connection setup;
					   request connection redirection if necessary*/
#define XTRCVCONNECT    7               /* fetch connection setup confirmation;
					   wait if necessary */
#define XTSNDDIS        8               /* request connection cleardown */
#define XTRCVDIS        9               /* fetch connection cleardown display */
#define XTSYNC          10              /* fetch TEP information */
#define XTGETSTATE      11              /* fetch TEP status */
#define XTSND           12              /* send normal or priority data to conn.
					   wait if necessary until it permits
					   dataflow control again*/
#define XTRCV           13              /* fetch normal or priority data on conn
					   wait if necessary until there is some
					   to fetch */
#define XTGETINFO       14              /* fetch information on TSP (transport
					   service provider) of a TEP */
#define XTOPEN          15              /* connect TEP to TSP (transport service provider)
					   and fetch information on TSP */
#define XTGETPROTADDR	16		/* get own and remote address */
#define XTOPTMGMT	17		/* negotiate options */
#define XT_NCMD         18              /* number of commands */

/*
 * Substructure for commands STINIRQ, STTRMRQ, XTBIND, XTUNBIND
 */
struct cx_apar_t
{
	char            *ap_pusapname;  /* Pointer to local name of applic.
					   in user address space */
	unsigned int    ap_lsapname;    /* Length of local name */
	unsigned short  ap_libversion;  /* Version of CMX-/XTI-library,
					   see end of cxioctl.h */
	unsigned short  ap_autversion;  /* Version of CMX/XTI automatons,
					   see end of cxioctl.h */
	char            ap_type;        /* Type of application */
/*                      0                  normal application */
#define CX_CMXD         1               /* CMX/XTI daemon */
#define CX_GWD          2               /* Gateway daemon */
	char            ap_res;         /* no significance */
#define ap_generate     ap_res          /* Automatic address generation? */
	unsigned short  ap_spspec;      /* restricted TSP features;
					   bit representation, ordered,
					   defined in cxioctl.h;
					   or 0, if no restriction */
	long            ap_sptypes;     /* TSP address types
					   bit representation, ordered,
					   def. at end of cxioctl.h */
	struct cc_list_t
			ap_cclist;	/* CC list */
	char            ap_signal;      /* required signal with announcement
					   of event (TEP specific !);
					   0, if no signalization required */
	char            ap_mode;        /* t_apmode (CMX) or insignificant */
	short           ap_nconcnt;     /* t_conlim (CMX) or qlen (XTI) */
	char		*ap_purpadd;	/* pointer to area for remote address */
	unsigned int	ap_lrpadd;	/* length of area for remote address */
};

/*
 * Substructure for commands STCON.., STDIS.., STRED.., XTCONNECT, XTLISTEN,
 * XTACCEPT, XTRCVCONNECT, XTSNDDIS, XTRCVDIS
 */
struct cx_cpar_t
{
	char            *cp_pusapname;  /* Pointer to local names of app.
					   in user address space */
	unsigned int    cp_lsapname;    /* Length of local name */
	long            cp_profil;      /* no significance; default 0 */
	short           cp_sres;        /* no significance; default 0 */
	unsigned short  cp_spspec;      /* restricting TSP features;
					   bit representation, ordered,
					   defined in cxioctl.h;
					   or 0, if no restriction */
	long            cp_sptypes;     /* TSP address types;
					   bit representation, ordered,
					   def. at end of cxioctl.h */
	struct cc_list_t
			cp_cclist;	/* CC list */
	long            cp_idumaxl;     /* maximum length of data unit
					   at transport service interface
					   (TIDU) */
	short           cp_maxinactsec; /* maximum inactivity time of conn.
					   in seconds (t_timeout with CMX);
					   0 and -1 mean unrestricted */
	unsigned short  cp_reason;      /* reason for connection cleardown */
	char            *cp_pumadd;     /* pointer to specified seperate tr.
					   address in user adress space */
	unsigned int    cp_lmadd;       /* length of this transport address */
	char            *cp_purmadd;    /* pointer to area for seperate
					   transport address to be fetched in the
					   user address space */
	unsigned int    cp_lrmadd;      /* length of this area or fetched
					   transport address */
	char            *cp_pupadd;     /* pointer to specified partner tr.
					   address in user address space */
	unsigned int    cp_lpadd;       /* length of this transport address */
	char            *cp_purpadd;    /* pointer to area for partner transport
					   address to be fetched in the user
					   address space */
	unsigned int    cp_lrpadd;      /* length of this area or fetched transport
					   address */
	char            *cp_puopt;      /* pointer to specified QOS parameter
					   set (in ITRANS-005 format, only comprises
					   QOS-TLV subparameters) in the user
					   address space */
	unsigned int    cp_lopt;        /* length of QOS parameter record */
	char            *cp_puropt;     /* pointer to area for QOS parameter set to
					   be fetched (im ITRANS-005
					   format, only comprises QOS-TLV subparameters)
					   in user address space */
	unsigned int    cp_lropt;       /* length of this area or fetched QOS-
					   parameter set */
	char            *cp_pudat;      /* pointer to user data to be set in user
					   address space */
	unsigned int    cp_ldat;        /* length of this user data */
	char            *cp_purdat;     /* pointer to area for user data to be
					   fetched in user address space */
	unsigned int    cp_lrdat;       /* Length of this area of received
					   user data */
};

/*
 * Substructure for the commands STDAT.., STXDT.., XTSND, XTRCV
 */
struct cx_dbpar_t                       /* substructure for an individual data
					   area in user address space */
{
	char            *dp_pu;         /* pointer to area with user data in
					   the user adress space to be sent
					   or for fetching */
	unsigned int    dp_l;           /* length of area or of fetched user
					   data in the area */
};
#define CX_DBUFSIZE	32
#define CX_NDBPAR	3
struct cx_dpar_t
{
	struct cx_dbpar_t dbp[CX_NDBPAR]; /* parameter for user areas in
					   user address space */
	char            db[CX_DBUFSIZE]; /* area for user data */
};

/*
 * Substructure for the commands STINFO, ST2INFO, XTOPEN, XTSYNC, XTGETINFO
 */
struct cx_ipar_t
{
	char            *ip_puservname; /* pointer to the name of transport
					   service in user address space */
	unsigned int    ip_lservname;   /* length of name */
	unsigned short  ip_libversion;  /* version of CMX-/XTI-library,
					   see end of cxioctl.h */
	unsigned short  ip_autversion;  /* version of CMX/XTI automatons,
					   see end of cxioctl.h */
	short           ip_sres;        /* insignificant */
	unsigned short  ip_spspec;      /* restricting TSP features;
					   bit representation, ordered,
					   defined in cxioctl.h;
					   or 0, if no restriction */
	long            ip_sptypes;     /* TSP address types;
					   bit representation, ordered,
					   def. at end of cxioctl.h */
	struct cc_list_t
			ip_cclist;	/* CC list */
	unsigned short  ip_ccptype;     /* CCP type, def. at end of cxioctl.h*/
	unsigned short  ip_ccpversion;  /* CCP version, as specified by CCP;
					   see in_version in cxitplus.h */
	unsigned short  ip_nsap;        /* max. number simultaneously existing
					   TSAPs of TSP */
	unsigned short  ip_ncep;        /* max. number simultaneously existing
					   TCEPs of TSP */
	long            ip_idumaxl;     /* max. length of data unit at
					   transport service interface
					   (TIDU) */
	long            ip_tmid;        /* ID of transport type, for
					   XTI currently always T_COTS,
					   insignificant for CMX */
	long            ip_sdumaxl;     /* maximum length of data unit
					   of transport service (TSDU);
					   -1, if length is unrestricted */
	long            ip_xsdumaxl;    /* maximum length of priority data
					   unit of trans. service (ETSDU);
					   -1, if length unrestricted;
					   -2, if not permitted */
	long            ip_conmaxl;     /* maximum length of user data with
					   connection setup request and
					   display;
					   -2, if not permitted */
	long            ip_confmaxl;    /* maximum length of user data
					   with connection setup acceptance
					   and confirmation;
					   -2, if not permitted */
	long            ip_dismaxl;     /* maximum length of user data with
					   connection setu]p request
					   and  display;
					   -2, if not permitted */
	long            ip_disfmaxl;    /* currently always -2 */
	long            ip_addmaxl;     /* maximum length of transport
					   address */
	long            ip_optmaxl;     /* maximum length of TSP options;
					   -1, if length is unrestricted;
					   -2, if not permitted */
	long		ip_flags;	/* provider flags, defined in xti.h */
};

/*
 * Substructure for commands XTOPTMGMT
 */
struct cx_opar_t
{
	long		op_action;	/* required action */
	long		op_reaction;	/* result */
	char		*op_puopt;	/* pointer to required options */
	unsigned int	op_lopt;	/* length of required options */
	char		*op_puropt;	/* pointer to returned options */
	unsigned int	op_lropt;	/* length of returned options */
};

/*
 * General argument structure for CMX- and XTI commands
 */
struct cx_upar_t
{
	char            up_subtype;     /* CX_UPAR, if no substructure; */
#define CX_UPAR 	0x10
					/* otherwise ID of substructure: */
#define CX_APAR         0x11            /* ID for cx_apar_t */
#define CX_CPAR         0x12            /* ID for cx_cpar_t */
#define CX_DPAR         0x13            /* ID for cx_dpar_t */
#define CX_IPAR         0x14            /* ID for cx_ipar_t */
#define CX_APAR0	0x15		/* cx_apar V3.0 (gateways)*/
#define CX_CPAR0	0x16		/* cx_cpar V3.0 (gateways)*/
#define	CX_OPAR		0x17		/* ID for cx_opar_t */
	char            up_n;           /* number of dbp[] */
	short           up_maxsec;      /* maximum wait time in seconds;
					   0 means no wait,
					   -1 means unrestricted wait */
	char            up_xdt;         /* priority data: 0 no, CX_XDT yes */
#define CX_XDT		2
	char            up_more;        /* [E]TSDU ends: 0 yes, CX_MORE no*/
#define CX_MORE 	1
	unsigned short  up_mult;        /* multipurpose field: */
#define up_re           up_mult         /* duplicate of response (except
					   T_NOEVENT) with ST*EVENT */
#define up_epid         up_mult         /* TEP ID with connection redirect.*/
#define up_reason       up_mult         /* reason for connection cleardown */
#define up_dbl          up_mult         /* length of user data in db
					   (struct cx_dpar_t) */
	char		up_mignea;	/* NEABX application (1) or not (0) */
	char		up_res;		/* void */
	short		up_sres;	/* void */
	unsigned int    up_l;           /* total length of user data */
	unsigned short  up_aattid;      /* logon ID specified by automaton */
	unsigned short  up_acepid;      /* TCEP ID specified by automaton */
	long            up_uattid;      /* logon ID specified by user */
	long            up_ucepid;      /* TCEP ID specified by user */
	union cx_upar_u
	{
		struct cx_apar_t ap;
		struct cx_cpar_t cp;
		struct cx_dpar_t dp;
		struct cx_ipar_t ip;
		struct cx_opar_t op;
	}
			upu;
};

/*
 * Length specifications for argument of CMX and XTI commands
 */
#define CX_USIZE	(sizeof(struct cx_upar_t) - sizeof(union cx_upar_u))
#define CX_ASIZE	(CX_USIZE + sizeof(struct cx_apar_t))
#define CX_CSIZE	(CX_USIZE + sizeof(struct cx_cpar_t))
#define CX_DSIZE	(CX_USIZE + sizeof(struct cx_dpar_t))
#define CX_ISIZE	(CX_USIZE + sizeof(struct cx_ipar_t))
#define CX_OSIZE	(CX_USIZE + sizeof(struct cx_opar_t))
#define CX_DBSIZE	(sizeof(struct cx_dbpar_t))
#define CX_DBASSIZE	(CX_DSIZE - CX_DBUFSIZE - CX_NDBPAR * CX_DBSIZE)
#define CX_DMIDSIZE	(CX_DSIZE - CX_DBUFSIZE)

/*
 * restricting TSP features (.._spspec)
 */
#define CX_SPNXDT       0x0001          /* TSP does not permit priority data */
#define CX_SPXDT        0x0002          /* TSP permits priority data */

/*
 * Macros to evaluate CC identifiers.
 */
#define	CX_CCSELBIT	0x0080		/* mask for "selected bit" */
#define CX_CCADID	0x000f		/* mask for adaptor identifier */
#define CX_CCINDEX	0xff00		/* mask for CC index specific to 
					   adaptor */
#define CX_GCCADID(ccid) ((ccid) & CX_CCADID)
#define CX_GINCC(ccid)	 ((int)((ccid) & CX_CCINDEX) / 256)

/*
 * Adaptor identifier values.
 */
#define CX_WANADID	0x0000		/* Id. of WAN-CC adaptor */
#define CX_LANADID	0x0001		/* Id. of LAN-CC adaptor */
#define CX_LBS2ADID	0x0002		/* Id. of local BS2000 adaptor */
#define CX_LOOPADID	0x0003		/* Id. of loop adaptor */
#define CX_TPIADID	0x0004		/* Id. of TPI adaptor */
#define CX_MB2ADID	0x0005		/* Id. of Multibus 2 adaptor */
/*
 * Pseudo adaptors. These values are only needed by the ICMX(L) library; 
 * they have no semantics for the CMX automaton.
 */
#define	CX_XTIID	0x000d		/* ICMX(L) on top of XTI */
#define	CX_LAN1ID	0x000e		/* ICMX(L) on top of TCP/IP */
#define	CX_SNAID	0x000f		/* SNA communication */

/*
 * CCP types (cc_type)
 */
#define CX_WAN1         0x01            /* WAN1 as of V2.0 */
#define CX_LAN2         0x22            /* LAN2 as of V2.0 */
#define CX_LBS2         0x60            /* local BS2000 adapter */
#define CX_LOOP         0x80            /* Loop adapter */
#define CX_TPIA         0xa0            /* TPI adapter */
#define CX_CCPUNSPEC    0x00            /* other */

#define CX_ALLSPTYPES	0x7fffffff

/*
 * Version specifications
 *
 * General setup: 0xTVSR with
 * T = system type (0 = computer-type dependent
 *                 1 = MX300 and derivates (CMX-H)
 *                 2 = X20         (CMX-I)
 *		   3 = MX300I      (CMX-L)
 *                 4 = WX200       (CMX-K)
 *		   6 = Targon 4600 (CMX-P)
 *		   7 = MS-DOS
 *		   8 = MX500I      (CMX-M)
 *                 9 = RM700I      (CMX-T)
 *                 a = WX200R      (CMX-N)
 *                 b = X10         (CMX-B)
 *                 c = MX2 and derivatives (CMX-C)
 *		   d = WX200 with Sinix V5.41 (CMX-D)
 *                 e = RM700R      (CMX-Y)
 *		   f = MX500       (CMX-F)
 * V = Version "in front of period",
 * S = Version "after period",
 * R = Revision (a, b, c, d, e, f).
 *
 * If a component in a version is not modified in relation to an interface,
 * it can maintain the previous version specification.
 *
 * Examples:
 * 0xc201  Version specification of CMX-V2.0 and V2.1 library
 *         (not totally in compliance with above rules)
 * 0x025a  Version specification of CMX-att-V2.5 and XTI-V1.0 library
 *         (the XTI library uses the specification of the corresponding
 *          CMX version)
 * 0x125a  Version specification of CMX/XTI automatons of CMX-V2.5 for MX300
 */
#define CX_VERSTYPE     0xf000          /* Mask for system type in v. spec. */
#define CX_VERSREST     0x0fff          /* Mask for rest of version spec. */

/*
 * Constant, which the XTI module of the CMX/XTI automatons adds to the XTI
 * error code and the XTI library program subtracts again.
 */
#define XT_ERRORBASE	100
#endif /* _CXIOCTL_H */
