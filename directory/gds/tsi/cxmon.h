/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cxmon.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:45  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:03  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:49:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:57  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:18:01  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:06  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:56  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:30  marrek]
 * 
 * $EndLog$
 */
#ifndef _CXMON_H
#define _CXMON_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cxmon_rcsid[] = "@(#)$RCSfile: cxmon.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:45 $";
#endif

/* #ident "@(#)cxmon.h	311.6 93/08/24 CMX (NC1)" */

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
 * ioctl constants and structures of ICMX(S3) for administration.
 *
 * (see also c*ioctl.h for the CC adapter
 *  and cxtioctl.h for the CMX/XTI atomaton)
 *
 * General form of system call:
 *
 * ioctl(fd, cmd, parg)
 *	int	fd;	file descriptor
 *      int     cmd;    command ID
 *      ...     *parg;  pointer to argument
 *
 * Ammendment status:
 * RK	(NC152)	03.07.91	Basic version CMX V3.1 (<- 3.6).
 * RK	(NC152)	27.11.91	ioctl(CX_SINFO) added.
 * RK	(NC152)	29.11.91	ioctl(CX_SINFO) as reviewed.
 * RK	(NC152)	03.02.92	CC lists replace CC bits; cxc_ppadd removed.
 * RK	(NC152)	11.03.92	CX_EREAD and CX_EWRITE defined.
 * RK	(NC152)	05.04.92	Merging cx_cep_t and cx_scepmon.
 * RK	(NC152)	11.01.93	CX_NSP = 32.
 */

/*
 * Maximum values for statically configured tables.
 * NOTE: machine-specific values defined in conf_cx.c must not exceed 
 * this definition!
 */
#define CX_NSP          32              /* Maximum number of TSPs */

/*
 * Border between "short" and "long" data for monitor
 */
#define CX_SDATLIMIT	32

/*
 * Help for filling out and evaluating double fields (long ...[2])
 */
#define CX_TL1FACTOR	1000000000L
#define CX_STL(ptl,l)				\
{						\
	if (((ptl)[0] += (l)) >= CX_TL1FACTOR)	\
	{					\
		(ptl)[0] -= CX_TL1FACTOR;	\
		(ptl)[1]++;			\
	}					\
}

#ifndef CX_ATTCC                        /* if not defined in cmxss2.h */
/*
 * General applicable ioctl argument structure
 */
struct udat
{
	char            *udat_p;        /* Data address
					   (in user address space) */
	long            udat_l;         /* Data length */
	long            udat_offl;      /* Distance of data from beginning of
					   area (area cmd-specific) */
	long            udat_spec;      /* cmd-specific addition */
};
#endif

/*
 * Constants for ordering the IDs of table commands of CC adapters
 * (see c[dlw]ioctl.h) and CMX/XTI automaton (see cxtioctl.h)
 */
#define CX_IOWRITE      0x8000          /* turns a "read" into a
					   "write command" */
#define CX_IOTAB        0x0080          /* indicates that udat_spec contains
					   the length of a table element */

/*
 * ID of commands for trace operation and suppressing the time monitoring
 * (clock) at CC adapter and CMX/XTI automaton (test device file);
 * the structure of ioctl argument (*parg) is struct udat (s.o.), their
 * application depends on commands (see individual description, structure
 * elements not mentioned are insignificant);
 * response 0, if no errors
 */
#define CX_GVERSION	CX_IO('x',1)	/* udat_spec = CC-Id. << 16 | Version
					   of CC adapter or = set version of
					   CMX/XTI automaton */
#define CX_GTSWITCH     CX_IO('x',2)    /* udat_spec = set trace switch */
#define CX_STSWITCH     CX_IO('x',3)    /* set trace switch = udat_spec */
#define CX_TSTART       CX_IO('x',4)    /* switch on trace */
#define CX_TSTOP        CX_IO('x',5)    /* stop trace */
#define CX_TREAD        CX_IO('x',6)    /* read trace list elements at the
					   udat_p address;
					   udat_spec in the two lower value
					   bytes first contains the maximum
					   number of elements to be read, and then
					   the number of actually read
					   elements */
#define CX_TEND         CX_IO('x',7)    /* Switch off trace */
#define CX_UNCLOCK      CX_IO('x',8)    /* Stop clock */
#define CX_RECLOCK      CX_IO('x',9)    /* Allow clock to continue */
#define CX_MEASON       CX_IO('x',0x40) /* Switch on time measurement */
#define CX_MEASOFF      CX_IO('x',0x41) /* Switch off time measurement */
#define CX_TINFO        CX_IO('x',0x42) /* Inform of trace list;
					   udat_spec in the two lower value
					   bytes, the number of trace list
					   elements of a list half,in the next
					   higher byte, the element length and
					   in the highest byte the status of
					   the tracer daemons (see li_daemstat
					   in struct lima in cxlima.h) */
#define CX_TDAEMSTART   CX_IO('x',0x44) /* Switch on trace and logon trace
					   daemon */
#define CX_TDAEMSTOP    CX_IO('x',0x45) /* Arrage for trace daemon to end
					   itself */

/*
 * IDs of command for CC and CCP administration
 * at CC adapter (administration device file);
 * ioctl parameter parg is insignificant, except with CX_READYCCP3 (see above);
 * Response 0, if errorless
 */
#ifndef CX_ATTCC                        /* if not defined in cmxss2.h */
#define CX_ATTCC        CX_IO('C',2)    /* logon CC with CMX */
#endif
#ifndef CX_DETCC                        /* if not defined in cmxss2.h */
#define CX_DETCC        CX_IO('C',3)    /* log off CC with CMX */
#endif
#define CX_READYCCP     CX_IO('C',1)    /* report CCP ready to operate */
#define CX_READYCCP3    CX_IO('C',17)   /* report CCP ready to operate and
					   report its network address(es) to automaton;
					   the argument (*parg) is an address list of
					   type CX_NADDS (see below) and is filled out
					   by the caller with the network address(es) */

/*
 * Structures for forming an address list for command CX_CCINFO3
 *
 * An address list comprises the header (struct cx_adds)
 * and one or more addresses each with a header (struct cx_sadd).
 * Distance between the beginning of a header and the beginning of this
 * header type must be divisible by 4, i.e. follow an individual address
 * if necessary 1 to 3 fuel bytes, whose contents is 0.
 */
struct cx_adds                          /* header of structure list */
{
	char            cx_addsmode;    /* structure ID: */
#define CX_ADDSMODE     3
	char            cx_addstype;    /* Type of listed addresses: */
#define CX_NADDS        0x30            /* SBKA network address, beginning with TyNA
					   (0x30) */
#define CX_TADDS        0xb0            /* SBKA transport address, comprising
					   SBKA network address (beginning with TyNA
					   (0x30)) and TSAP-Id (begining with
					   TyTS (0x80));
					   this type is currently not used */
	unsigned short  cx_addsl;       /* total length of address list inc. header */
};
#define CX_ADDS_S	(sizeof(struct cx_adds))

struct cx_sadd                          /* Header of an individual address */
{
	char            cx_saddspsel[4];/* Field for TSP ID (later),
					   all bytes currently 0 */
	long            cx_saddsptype;  /* TSP address type (only a !),
					   def. at end of cxioctl.h */
	unsigned short  cx_saddl;       /* Length of subsequent SBKA network
					   or transport address */
	unsigned short  cx_saddres;     /* Align to long.      */
};
#define CX_SADD_S	(sizeof(struct cx_sadd))
/*
 * Macros for accessong header of individual address
 * (mit struct cx_adds *padds, struct cx_sadd *psadd)
 */
#define CX_FSADD(padds) ((struct cx_sadd *)((padds) + 1))
					/* Header of first address */
#define CX_NSADD(psadd) ((struct cx_sadd *)((char *)((psadd) + 1) + \
		 ((((int)((psadd)->cx_saddl) + 3)>>2)<<2)))
					/* Header of next address */
 
/*
 * IDs of command for CMX administration
 * at CMX/XTI automaton (administration device file);
 * structure of ioctl argument (*parg) depends on commands (see above);
 * Response 0 or (with CX_ERRINFO) >= 0, if no errors
 */
#define CX_CCINFO       CX_IO('C',4)    /* Fetch infor of all CMX/XTI operable
					   WAN- and LAN-CCs (restricted due to
					   CCPCON); argument has the structure
					   cx_ccinfo (see above), filled out
					   by automaton */
#define CX_DINFO        CX_IO('C',5)    /* Fetch info on CMX/XTI automaton;
					   the argument has the structure
					   cx_dinfo (see below) filled out by
					   automaton */
#define CX_CCMON        CX_IO('C',6)    /* Fetch monitor data of all CCs operated
					   by CMX/XTI; the argument has the
					   structure cx_ccmon (see below) and
					   is filled out by the automaton */
#define CX_DMON         CX_IO('C',7)    /* Fetch monitor data on CMX/XTI automaton;
					   the argument has the structure
					   cx_dmon (see below) and is filled
					   out by automaton */
#define CX_ERRINFO      CX_IO('C',10)   /* Fetch contents of error message field:
					   the argument is a table with
					   CX_NERRINFO elements of the structure
					   cx_errinfo (see below); the automaton fills
					   out as many of these as are filled out
					   in the error message field; it returns
					   the number */
#define CX_UNPANIC      CX_IO('C',11)   /* reset "soft panic" of automatons; parg
					   is insignificant */
#define CX_CCINFO2      CX_IO('C',12)   /* Fetch info on all CMX/XTI operable
					   CCs and pseudo-CCs (see
					   CX_CCINFO); the argument has the
					   structure cx_ccinfo (see below) and is filled
					   out by the automaton */
#define CX_CCINFO3      CX_IO('C',13)   /* Fetch info on all CMX/XTI operable
					   CCs and pseudo-CCs and fetch its TSPs
					   (transport service providers);
					   the argument has the structure
					   cx_ccinfo3 (see below) and is filled out
					   by the automaton */
#define CX_SINFO        CX_IO('C',16)   /* Fetch information on TSAPs, TEPs, and
					   TCEPs; The structure cx_xinfo is assigned
					   by automatons and the specified buffer filled
					   out with TLV parameters of the cx_sinfo,
					   cx_pfinfo, and cx_tinfo,
					   (see also cxtransview.h) */

/*
 * Structures and constants for commands CX_CCINFO and CX_CCINFO2:
 * cx_ccinfo  -  total structure
 * cx_binfo   -  substructure for a CC, provided by CC adapter
 * cx_pinfo   -  substructure for a CC, provided by CMX/XTI automaton
 *
 * Structures and constants for command CX_CCINFO3:
 * cx_ccinfo3 -  total structure
 * cx_binfo   -  substructure for a CC, provided by CC adapter
 * cx_pinfo3  -  substructure for a CC, provided by CMX/XTI automaton
 * cx_spinfo3 -  substructure for a TSP (transport service provider)
 */
struct cx_binfo
{
	unsigned short	cxb_id; 	/* CC identifier, see cxioctl.h */
	char            cxb_stat;       /* status of CC: */
#define CX_CCEXIST      1               /* CC available,
					   but not logged on with adapter */
#define CX_CCEX 	CX_CCEXIST
#define CX_CCATT        2               /* CC logged on with adapter,
					   but CCP not ready to operate */
#define CX_CCPREADY     3               /* CCP ready to operate */
	char		cxb_res;	/* reserved, always 0 */
	unsigned short  cxb_adversion;  /* version of CC operating CC adapter,
					   see end of cxioctl.h */
	unsigned short  cxb_busid;      /* ID of bus over which the CC is
					   connected; e.g. SLIC address with
					   MX500 */
	unsigned short  cxb_intid;      /* interruption ID of CC; e.g.
					   Multibus interrupt with MX2, MX300
					   and MX500 */
	short		cxb_res2;	/* reserved, always 0 */
	long            cxb_add;        /* address of control and status
					   register of CC; e.g. Multibus-IO
					   address with MX2, MX300 and MX500 */
	long            cxb_memsize;    /* Size of CC memory in bytes */
	long            cxb_hwversion;  /* HW version, as specified by CC */
	long            cxb_fwversion;  /* FW version, as specified by CC */
};

#define CX_SSIZE	80
struct cx_pinfo
{
	char            cxp_res;        /* always 0 */
	char            cxp_stat;       /* status:
					   0, if the remaining contents of the
					   elements is invalid;
					   1, if it is valid */
	unsigned short  cxp_sptype;     /* address type of (only) TSP of CCP,
					   def. at end of cxioctl.h,
					   or 0 with CCPs >= V3.0 */
#define cxp_addtype	cxp_sptype
	unsigned short  cxp_type;       /* CCP type, def. at end of cxioctl.h;
					   or 0 for older CCPs */
	unsigned short  cxp_version;    /* CCP version, as specified by CCP;
					   for newer CCPs see "Version specification"
					   at end of cxioctl.h */
	unsigned short  cxp_nsap;       /* maximum number of simultaneously existing
					   TSAPs of CCP */
	unsigned short  cxp_ncep;       /* maximum number of simultaneously existing
					   TCEPs of CCP */
	unsigned short  cxp_datmaxl;    /* maximum length of a data unit at the
					   transport service interface
					   (TIDU) */
	unsigned short  cxp_xdtmaxl;    /* maximum length of a priority data unit of
					   the transport service (ETSDU);
					   0, if priority data not permitted;
					   or always 0 with CCPs >= V3.0 */
	unsigned short  cxp_conmaxl;    /* maximum length of user data with
					   connection setup requirement;
					   or 0 with CCPs >= V3.0 */
#define cxp_crqmaxl	cxp_conmaxl
	unsigned short  cxp_confmaxl;   /* maximum length of user data with
					   connection setup confirmation;
					   or 0 with CCPs >= V3.0 */
#define cxp_crsmaxl	cxp_confmaxl
	unsigned short  cxp_dismaxl;    /* maximum length of user data with
					   connection cleardown;
					   or 0 with CCPs >= V3.0 */
#define cxp_drqmaxl	cxp_dismaxl
	unsigned short  cxp_sl;         /* length of filled out part of subsequent
					   field (currently always 0) */
	char            cxp_s[CX_SSIZE];/* field for information taken directly
					   from CCP (currently insignificant) */
};

struct cx_pinfo3
{
	char            cxp_res;        /* always 0 */
	char            cxp_stat;       /* status:
					   0, if the remaining contents of the
					   element is invalid;
					   1, if valid */
	char            cxp3_nsp;       /* maximum number of simultaneously operable
					   TSPs of CCP */
	char            cxp3_subrevision; /* corrected version (binary) of CCP >=
					   V3.0, otherwise 0 */
	unsigned short  cxp_type;       /* CCP type, def. at end of cxioctl.h;
					   or 0 for older CCPs */
	unsigned short  cxp_version;    /* CCP version, as specified by CCP;
					   for more recent CCPs see "Version
					   specifications" at the end of cxioctl.h */
	unsigned short  cxp_nsap;       /* maximum number of simultaneously operable
					   TSAPs of CCP */
	unsigned short  cxp_ncep;       /* maximum number of simultaneously operable
					   TCEPs of CCP */
	unsigned short  cxp_datmaxl;    /* maximum length of a data unit at the
					   transport service interface
					   (TIDU) */
	unsigned short  cxp3_sl;        /* length of filled out part of
					   subsequent field (currently always 0) */
	char            cxp3_s[CX_SSIZE];/* field for information, taken directly from CCP
					   (currently insignificant) */
};

struct cx_spinfo3
{
	unsigned short  cxsp3_ccid;     /* identifier of CC this TSP belongs
					   to,; see cxb_id */
	char            cxsp3_stat;     /* status:
					   0, if the remains contents of the
					   element is invalid;
					   1, if it is valid */
	char            cxsp3_sres;     /* always 0 */
	char            cxsp3_sel[4];   /* TSP ID for CCPs >= V3.0,
					   otherwise all bytes 0 */
	long            cxsp3_types;    /* address type(s) of TSP,
					   bit representation, ordered,
					   def. at end of cxioctl.h */
	unsigned short  cxsp3_xdtmaxl;  /* maximum length of a priority data unit
					   of transport provider (ETSDU);
					   0, if priority data not permitted */
	unsigned short  cxsp3_conmaxl;  /* maximum length of user data
					   with connection setup requirement */
	unsigned short  cxsp3_confmaxl; /* maximum length of user data
					   with connection setup confimration */
	unsigned short  cxsp3_dismaxl;  /* maximum length of user data with
					   connection cleardown */
};

#define CX_NCCINFO	CX_NCC
struct cx_ccinfo
{
	unsigned short  cxcc_version;   /* Version of CMX/XTI automatons,
					   see end of cxioctl.h */
#define cxcc_sres	cxcc_version
	char            cxcc_res;       /* always 0 */
	char            cxcc_ncc;       /* number of automaton operated CCs
					   = number of filled out cx_sccinfo-
					   substructures */
	struct cx_sccinfo
	{
		struct cx_binfo cxb;
		struct cx_pinfo cxp;
	}
			cxcc[CX_NCC];
};

struct cx_ccinfo3
{
	unsigned short  cxcc_version;   /* version of CMX/XTI automatons,
					   see end of cxioctl.h */
	char            cxcc_nsp;       /* maximum number of simultaneous automoton
					   operable TSPs
					   = number of filled out cx_spinfo3-
					   substructures */
	char            cxcc_ncc;       /* number of automaton operated CCs
					   = number of filled out cx_sccinfo3-
					   substructures */
	struct cx_spinfo3 cxsp3[CX_NSP];
	struct cx_sccinfo3
	{
		struct cx_binfo cxb;
		struct cx_pinfo3 cxp3;
	}
			cxcc3[CX_NCC];
};

/*
 * Structure for command CX_DINFO
 */
struct cx_dinfo
{
	unsigned short  cxd_version;    /* Version of CMX/XTI automatons,
					   see end of cxioctl.h */
	char            cxd_res;        /* always 0 */
	char            cxd_ncc;        /* number of CMX operated CCs */
	unsigned short  cxd_natt;       /* maximum number of simultaneous loggons
					   with CMX/XTI automaton */
	unsigned short  cxd_anatt;      /* number of actually existing logons with the
					   CMX/XTI automaton */
	unsigned short  cxd_nsap;       /* maximum number of simultaneously logged on
					   applications */
	unsigned short  cxd_ansap;      /* number of actually logged on
					   applications */
	unsigned short  cxd_ncep;       /* maximum number of simultaneously existing
					   TCEPs */
	unsigned short  cxd_ancep;      /* number of actual existing
					   TCEPs */
	unsigned short  cxd_nep;        /* maximum number of simultaneouly existing
					   TEPs */
#define cxd_nproc	cxd_nep
	unsigned short  cxd_anep;       /* number of actual existing
					   TEPs */
#define cxd_anproc	cxd_anep
	unsigned short  cxd_nsp;        /* maximum number of simultaneously operable
					   TSPs (transport service provider) */
#define cxd_nred	cxd_nsp
	unsigned short  cxd_ansp;       /* number of ready to operate
					   TSPs (transport service provider) */
#define cxd_anred	cxd_ansp
};

/*
 * Structures and constants for command CX_CCMON:
 * cx_ccmon   -  total structure
 * cx_bmon    -  substructure for a CC, provided by CC adapter
 * cx_pmon    -  substructure for a CC, provided by CMX/XTI automaton
 */
struct cx_bmon
{
	unsigned short	cxb_id; 	/* CC identifier, see cxioctl.h */
	char            cxb_stat;       /* status of CC (see struct cx_binfo)
					   or 0, if the remaining constants
					   of the element is invalid */
	char		cxb_res;	/* reserved, always 0 */
	unsigned short  cxb_adversion;  /* version of the CC operating CC-
					   adapter, see end of cxioctl.h */
	short		cxb_res2;	/* reserved, always 0 */
	long            cxb_lres;       /* insignificant */
	struct cx_time  cxb_loadtime;   /* time of beginning of last
					   CCP load */
					/* total length and number since: */
	long            cxb_tnspstart;  /* number of adapter interruptions -> CCP
					   for transport service provider */
	long            cxb_tnspintr;   /* number of interruptions CCP -> adapter
					   for transport service provider */
	long            cxb_tnadstart;  /* number of interruptions -> CCP
					   for adm./diag. */
	long            cxb_tnadintr;   /* number of interruptions -> CCP -> adapter
					   for adm./diag. */
	long            cxb_lres2;      /* insignificant */
	long            cxb_lres3;      /* insignificant */
	long            cxb_tnadsend;   /* no. of adm./diag. send commands */
	long            cxb_tnadget;    /* no. of adm./diag. fetch commands */
	long            cxb_tladsend[2];/* length of sent adm./diag. data,
					   [1] * CX_TL1FACTOR + [0] */
	long            cxb_tladget[2]; /* length of fetched adm./diag. data,
					   [1] * CX_TL1FACTOR + [0] */
};

struct cx_pmon
{
	char            cxp_res;        /* always 0 */
	char            cxp_stat;       /* status:
					   0, if the remaining contents of the
					   element is invalid;
					   1, if it is valid */
	unsigned short  cxp_sptype;     /* address type of (only) TSP of CCP,
					   def. at end of cxioctl.h,
					   or 0 with CCPs >= V3.0 */
	unsigned short  cxp_type;       /* CCP type, def. at end of cxioctl.h*/
	unsigned short  cxp_version;    /* CCP version, as specified by CCP;
					   for more recent CCPs see "Version
					   specifications at end of cxioctl.h */
	unsigned short  cxp_nsap;       /* maximum number of simultaneously existing
					   TSAPs of CCP */
	unsigned short  cxp_ncep;       /* maximum number of simultaneously existing
					   TCEPs of CCP */
	unsigned short  cxp_ansap;      /* number of actually existing TSAPs
					   of CCP */
	unsigned short  cxp_ancep;      /* number of actually existing TCEPs
					   of CCP */
	short           cxp_anspbusy;   /* number of all commands for
					   transport service provider;
					   with which editing the CC is currently
					   occupied */
	short           cxp_maxnspbusy; /* maximum number of commands for
					   transport service provider;
					   with which editing of CC
					   was simultaneously occupied */
	long            cxp_tnconrs;    /* number of connection setup
					   acceptance (CONRS) */
	struct cx_time  cxp_readytime;  /* time from when the CCP is ready to
					   operate */
	struct cx_time  cxp_endtime;    /* time up to which the CCP can
					   operate, or 0 */
					/* total numbers and lengths since beginning
					   of CCP operability
					   ( cxp_tnconrs also belongs): */
	long            cxp_tnsap;      /* number of TSAP configurations */
	long            cxp_tncep;      /* number of TCEP configurations */
	long            cxp_tnconcf;    /* number of connections setup
					   confirmations  */
	long            cxp_tnerrdisin; /* number of displays from connection
					   setups (DISIN due to errors) */
	long            cxp_tnfconrq;   /* number of refused connected setup
					   requests (CONRQ with no CONCF) */
	long            cxp_tnfconin;   /* number of refused connection setup
					   displays (CONIN with no CONRS) */
	long            cxp_tnsignal;   /* number of signalized
					   transport events */
	long            cxp_tnselect;   /* number of select() awakening
					   transport events */
	long            cxp_tneventw;   /* number of cx_event() awakening
					   transport events */
	long            cxp_tnspbusy;   /* number of all commands for
					   transport service providers
					   with arrival of CC was busy with the
					   editing of another such command */
	long            cxp_tnspsend;   /* number of all send commands for
					   transport service provider */
	long            cxp_tnspgetquit;/* number of all receive commands for
					   transport service provider */
	long            cxp_tnflowsend; /* number of sent data flow control
					   commands */
	long            cxp_tnflowann;  /* number of announced data flow control
					   commands */
	long            cxp_tnsdatsend; /* number of data send commands with
					   data length <= CX_SDATLIMIT */
	long            cxp_tnsdatget;  /* number of data fetch commands with
					   data length <= CX_SDATLIMIT */
	long            cxp_tnldatsend; /* number of data send commands with
					   data length > CX_SDATLIMIT */
	long            cxp_tnldatget;  /* number of data fetch commands with
					   data length > CX_SDATLIMIT */
	long            cxp_tldatsend[2];/* length of data send commands of sent
					   user data,
					   [1] * CX_TL1FACTOR + [0] */
	long            cxp_tldatget[2];/* length of user data fetched with
					   data fetch commands,
					   [1] * CX_TL1FACTOR + [0] */
};

struct cx_ccmon
{
	unsigned short  cxcc_version;   /* version of the CMX/XTI automaton,
					   see cxioctl.h */
	char            cxcc_res;       /* always 0 */
	char            cxcc_ncc;       /* number of automaton operated CCs
					   = number of filled out cx_sccmon-
					   substructured */
	long            cxcc_lres;      /* insignificant */
	struct cx_time  cxcc_ctime;     /* current time */
	struct cx_sccmon
	{
		struct cx_bmon cxb;
		struct cx_pmon cxp;
	}
			cxcc[CX_NCC];
};

/*
 * Struktur fuer command CX_DMON
 */
struct cx_dmon
{
	struct cx_dinfo cxd;
	struct cx_time  cxd_ctime;      /* current time */
	struct cx_time  cxd_btime;      /* start time */
					/* total numbers and length since: */
	long            cxd_tnatt;      /* number of logons */
	long            cxd_tnsap;      /* number of app. configurations */
	long            cxd_tncep;      /* number of TCEP configurations */
	long            cxd_tnep;       /* number of TEP configurations */
	long            cxd_tnann;      /* number of all announcements */
	long            cxd_tnannnomem; /* number of announcements of connection
					   setup displays, that were not accepted,
					   since the TCEP table was fully occupied */
	long            cxd_tnop;       /* number of all open calls for network
					   device files */
	long            cxd_tnopnomem;  /* number of open calls for network device
					   files, that were refused, since the
					   TEP table was fully occupied
					   */
	long            cxd_tnopbusy;   /* number of open calls for network device
					   files that were refused, because the
					   specified network device file was already
					   opened */
	long            cxd_tnio;       /* number of all ioctl calls for network
					   device files */
	long            cxd_tnionomem;  /* number of ioctl calls for network device
					   files that were refused, because the logon,
					   application or TCEP table was fully occupied */
	long            cxd_tnioccerr;  /* number of ioctl calls for network device
					   files that were refused, because a transport
					   service provider was not operable
					   (any longer) */
	long            cxd_tniocoll;   /* number of ioctl calls for network device
					   files that were refused due to an
					   announced connection cleardown, but is
					   not known to the user */
	long            cxd_tniouserr;  /* number of ioctl calls for network device
					   files that were refused due to other
					   errors */
	long            cxd_tnconcf;    /* number of connection setup
					   confirnations */
	long            cxd_tnerrdisin; /* number of displays of connection cleardowns
					   not required by the partner
					   (DISIN due to errors) */
	long            cxd_tnfconrq;   /* number of refused connection setup requirements
					   (CONRQ with no CONCF) */
	long            cxd_tnfconin;   /* number of refused connection setup
					   displays (CONIN with no CONRS) */
	long            cxd_tnsignal;   /* number of signalized
					   transport events */
	long            cxd_tnselect;   /* number of select() awakening
					   transport events */
	long            cxd_tneventw;   /* number of cx_event() awakening
					   transport events */
	long            cxd_tnconrs;    /* number of connection setup
					   acceptances (CONRS) */
	long            cxd_tnspsend;   /* number of all send commands for
					   transport service provider */
	long            cxd_tnspgetquit;/* number of all receive commands for
					   transport service provider */
	long            cxd_tnflowsend; /* number of sent data flow control
					   commands */
	long            cxd_tnflowann;  /* number of announced data flow control
					   commands */
	long            cxd_tnsdatsend; /* number of data send commands with
					   data length <= CX_SDATLIMIT */
	long            cxd_tnsdatget;  /* numer of data fetch commands with
					   data length <= CX_SDATLIMIT */
	long            cxd_tnldatsend; /* number of data send commands with
					   data length > CX_SDATLIMIT */
	long            cxd_tnldatget;  /* number of data fetch commands with
					   data length > CX_SDATLIMIT */
	long            cxd_tldatsend[2];/* length of user data sent with
					   data send commands,
					   [1] * CX_TL1FACTOR + [0] */
	long            cxd_tldatget[2];/* length of user data fetched with data
					   fetch commands,
					   [1] * CX_TL1FACTOR + [0] */
};

/*
 * Structure of element of error message field (command CX_ERRINFO)
 */
#define CX_NERRINFO     256             /* number of elements */
struct cx_errinfo
{
	unsigned char   cxe_id;         /* entry marker (!= 0), see below */
#define CX_EFPANIC      0x40            /* ordered with "soft panic" */
#define CX_EFNOTFIRST   0x80            /* ordered with subsequent entry */
	char		cxe_res;	/* reserved */
	unsigned short	cxe_ccid;	/* CC identifier */
	unsigned short  cxe_pid;        /* ID of affected process,
					   or 0 */
	unsigned short  cxe_mindev;     /* number of affected network device
					   file (minor device number), or 0xffff */
	unsigned short  cxe_acepid;     /* ID specified by affected TCEP
					   TCEP (tref/sequence), or 0xffff */
	char            cxe_cmdid;      /* command number from the ICMX(S3)-
					   cmmand ID (see below), or 0 */
	char            cxe_cmdchar;    /* command letter from the ICMX(S3)-
					   command ID (see below) or 0 */
	unsigned short  cxe_error;      /* error code from errno.h, cx.h, cmx.h
					   or xti.h + XT_ERRORBASE (see below) */
	long            cxe_lvar;       /* depends on entry ID */
};

/*
 * error message field entry ID
 */
#define CX_EAM1 	0x01
#define CX_EAM2 	0x02
#define CX_EAM3 	0x03
#define CX_EAM4 	0x04
#define CX_EAM5 	0x05
#define CX_EAM6 	0x06
#define CX_EAM7 	0x07
#define CX_EOPEN	0x09
#define CX_EIOCTL	0x0a
#define CX_ESELECT	0x0b
#define CX_EANN 	0x0c
#define CX_EEND 	0x0f
#define CX_EINIT	0x10
#define CX_EFLOWSEND	0x11
#define CX_ESAPSEND	0x12
#define CX_ESAPGET	0x13
#define CX_ECEPSEND	0x14
#define CX_ECEPGET	0x15
#define CX_EDATSEND	0x16
#define CX_EDATGET	0x17
#define	CX_EREAD	0x18
#define	CX_EWRITE	0x19

/*
 * ioctl() parameter area for CX_SINFO.
 * The TLV structures put into cxx_buf are described in cxtransview.h!
 */
struct cx_xinfo
{
        unsigned short      cxx_version;     /* version of CMX automaton */
        unsigned short      cxx_ret;         /* return status: */
#define CX_SINFO_OKAY       0                /* no error */
#define CX_SINFO_NOSPACE    0xffff           /* buffer too small */
        unsigned long       cxx_tsid;        /* TSAP id. or CX_SINFO_ALL */
        unsigned long       cxx_tcid;        /* TCEP id. or CX_SINFO_ALL */
#define CX_SINFO_ALL        0xffffffff       /* inform about all objects */
        unsigned short      cxx_flags;       /* amount of detail desired: */
#define CX_SINFO_SHORT      0x0001           /* for all TSAPs specified, TSAP
                                                struct. and local name only */
        short               cxx_res1;        /* reserved */
        long                cxx_len;         /* data buffer length */
        long                cxx_rlen;        /* length of data in buffer */
        char                *cxx_buf;        /* pointer to data buffer */
};

/*
 * IDs of help commands for the CMX (as of V2.5) and XTI libraries
 * and for the CMX/XTI daemon
 * to CMX/XTI automaton (administration device file);
 * structure of the ioctl argument (*parg) depends on commands (see below)
 * response 0, if no errors
 */
#define CX_SPCONV(n)    CX_IO('V',n)    /* Loading of conversion table for the
					   names of transport services;
					   the argument is a conversion table
					   with n elements, which have the
					   cx_spconv structure (see below), and
					   is filled out with the CMX/XTI daemon
					   before the call*/
/*
 * The subsequent macros enable the CMX/XTI automaton to evaluate the commands
 * with variable components (CX_SPCONV(n)):
 */
#define CX_MASKN(cmd)   ((cmd)&0xffffff00) /* mask variable component */
#define CX_GN(cmd)      ((cmd)&0xff)    /* produces variable component */

/*
 * structure of element of argument for CX_SPCONV command
 * (appplication purpose: assignment between transport service name on the one
 * side and type(s) and if necessary restricted features of TSPs on the
 * other side)
 * TSP = transport service provider
 */
#define CX_SERVNAMESIZE 16
struct cx_spconv
{
	char            cxsp_servname[CX_SERVNAMESIZE]; /* space for the name of
					   the transport service incl. an end
					   null byte */
	char            cxsp_servid;    /* ID of transport service: */
#define CX_MSG          0               /* unspecified message orientated */
#define CX_OSI_COTS	1		/* ISO unspecified class */
#define CX_TP0		2		/* ISO class 0 */
#define CX_TP2		3		/* ISO class 2 */
#define CX_TP4		4		/* ISO class 4 */
#define CX_NEAT 	5		/* NEA */
#define CX_SINIX        6               /* local in SINIX system
					   (using Loop adapter) */
#define CX_STA1         7               /* station connection over CCP-STA1/MSV
					   (not for test purposes) */
	char            cxsp_tmid;      /* ID of transfer type: */
#define CX_COTS         1               /* connection-oriented */
	unsigned short  cxsp_spec;      /* restricted TSP features,
					   bit representation,
					   def. at end of cxioctl.h;
					   or 0, if no restriction */
	long            cxsp_types;     /* address types of (the) TSP(s);
					   bit representation, ordered,
					   def. at end of cxioctl.h */
};
#endif /* _CXMON_H */
