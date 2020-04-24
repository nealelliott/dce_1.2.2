/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndscfg.h,v $
 * Revision 1.1.159.2  1996/02/18  23:36:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:53  marty]
 *
 * Revision 1.1.159.1  1995/12/08  16:02:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:34  root]
 * 
 * Revision 1.1.157.4  1994/07/12  19:06:04  keutel
 * 	SVR4 conflict between xti.h and xti_inet.h
 * 	[1994/07/12  17:25:24  keutel]
 * 
 * Revision 1.1.157.3  1994/06/09  18:46:26  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:31  devsrc]
 * 
 * Revision 1.1.157.2  1994/05/10  15:56:11  marrek
 * 	April 1994 submission.
 * 	[1994/04/29  10:52:20  marrek]
 * 
 * Revision 1.1.157.1  1994/02/22  18:08:20  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:49:21  marrek]
 * 
 * Revision 1.1.155.2  1993/10/14  17:35:39  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:33:05  keutel]
 * 
 * Revision 1.1.155.1  1993/10/13  17:32:45  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  17:25:28  keutel]
 * 
 * $EndLog$
 */
#ifndef _NDSCFG_H
#define _NDSCFG_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _ndscfg_rcsid[] = "@(#)$RCSfile: ndscfg.h,v $ $Revision: 1.1.159.2 $ $Date: 1996/02/18 23:36:22 $";
#endif

#include <gds.h>
#include <netinet/in.h>
#include <xti.h>
#include <ndsif.h>
#include <dce/cmx_addr.h>

	/* This header file provides all informations about the NDS - */
	/* configuration data base (shared memory layout (SHM)) */

#define NDS_SHMVERS	1		/* version # of SHM-layout */


	/* NDS - SHM general area */

typedef struct {
	signed32	nds_versno ;	   /* version # of SHM-layout */
	signed32	nds_shmstate ;	   /* state of SHM-information */
	char		nds_conftitle[38] ;/* SHM-information title */
	signed32	nds_noifdesc ;	   /* # of transport interface */
					   /* description entries */
	signed32	nds_ifdescarea ;   /* offset (reference) to interface */
					   /* description area */
	signed32	nds_noruledesc ;   /* # of matching/mapping rule */
					   /* description entries */
	signed32	nds_ruledescarea ; /* offset (reference) to matching/ */
					   /* mapping rule description area */
	signed32	nds_rulearea ;	   /* offset (reference) to matching/ */
					   /* mapping rule area */
	signed32	nds_heap ;	   /* offset (reference) to heap area */
} Nds_shmgenarea ;


	/* NDS - SHM transport interface description entry */

typedef struct {
	signed32	nds_ifid ;	   /* transport interface-ID */
	signed32	nds_provid ;	   /* transport provider-ID */
	signed32	nds_profprioid ;   /* transport profile priority-ID */
	signed32	nds_devname ;	   /* offset (reference) to transport */
					   /* profile device name (optional) */
	signed32	nds_profopt ;	   /* offset (reference) to general */
					   /* transport profile option(s) */
} Nds_shmifdesc ;


	/* NDS - SHM matching/mapping rule description entry */

typedef struct {
	signed32	nds_mtype ;	   /* matching/mapping rule type */
	signed32	nds_nomrules ;	   /* # of matching/mapping rule entries */
	signed32	nds_mrules ;	   /* offset (reference) to matching/ */
					   /* mapping rules */
} Nds_shmruledesc ;


	/* NDS - SHM matching/mapping rule entry */

typedef struct {
	signed32	nds_mtchtype ;	   /* type and length of matching rule */
	signed32	nds_mtchrule ;	   /* offset (reference) to matching */
					   /* rule */
	signed32	nds_mprule ;	   /* offset (reference) to mapping */
					   /* rule */
	signed32	nds_ifref ;	   /* offset (reference) to correspon- */
					   /* ding interface description entry */
} Nds_shmrule ;

	/* Note: offset values are always relative to the SHM start address */
#define SHMREF(off)	((char *) nds_shm_base + off)

					/* error values */
#define NDS_DESCVERS_ERR	-90
#define NDS_DESCFREAD_ERR	-91
#define NDS_DESCMEM_ERR		-92
#define NDS_DESCFOPEN_ERR	-93
#define NDS_DESCSIZE_ERR	-94

					/* NSAP-address matching modes */
#define NDS_MATCH		1
#define NDS_MATCH_IF		2	/* match for a specific provider */
#define NDS_MATCH_PRIO		3	/* match for highest priority */

					/* NDS configuration data base file */
#define NDS_CONF_DIB		"NDSCONF.DIB"

#define NDS_DFLT_HPSIZE 	10240	/* default work space size */
#define NDS_FILLER		0xFF	/* alignment filler value */

					/* communication profile types */
#define NDS_RFC1006_PROF	0
#ifdef SNI_SINIX
#define NDS_OSITYPE_PROF	1
#define NDS_LANSBKA_PROF	2
#define NDS_WANSBKA_PROF	3
#define NDS_WAN3SBKA_PROF	4
#define NDS_WANNEA_PROF		5
#define NDS_LOOPSBKA_PROF	6
#define NDS_NETBIOS_PROF	7
#else
#define NDS_OSILAN_PROF		2
#define NDS_OSIWAN_PROF		3
#endif

					/* communication sub-profile types */
#define NDS_X121_SUBPROF	1
#define NDS_E163_SUBPROF	2
#define NDS_E164_SUBPROF	3
#define NDS_X31_SUBPROF		4
#define NDS_X32_SUBPROF		5
#define NDS_X21_SUBPROF		6
#define NDS_PVC_SUBPROF		7

					/* option representation formats */
#define NDS_EXP_ASCSTR	1		/* ASCII-string format (explicit) */
#define NDS_REC_ASCSTR	2		/*          "          (recurring) */
#define NDS_EXP_OCTSTR	3		/* OCTET-string format (explicit) */
#define NDS_REC_OCTSTR	4		/*          "        (recurring) */
#define NDS_EXP_SHORT	5		/* SHORT format (explicit) */
#define NDS_REC_SHORT	6		/*          "   (recurring) */
#define NDS_EXP_LONG	7		/* LONG format (explicit) */
#define NDS_REC_LONG	8		/*          "  (recurring) */
#define NDS_EXPREC_MSK  0x0F
#define NDS_IMP_DIGSTR	(1 << 4)	/* DIGIT-string        (implicit) */
#define NDS_IMP_OCTSTR	(2 << 4)	/* OCTET-string            "      */
#define NDS_IMP_INT     (3 << 4)	/* INTEGER                 "      */
#define NDS_IMP_MSK	(0xF << 4)

					/* NSAP-address specification */
#define NDS_GROUP_ADDR	(1 << 16)	/* group of addresses specified by */
					/* regular expression */
#define NDS_SINGLE_ADDR (2 << 16)	/* single address specified by octet */
					/* string */
#define NDS_ADDR_TY_MSK (0XF << 16)	/* type specification mask */	
#define NDS_ADDR_LN_MSK 0xFFFF		/* address length mask */

					/* option types */
#define NDS_SUBPROF_OPT	  (1 << 0)	/* sub-profile */
#define NDS_IP_OPT	  (1 << 1)	/* IP-address */
#define NDS_PORT_OPT	  (1 << 2)	/* port number/T-selector */
#define NDS_MAC_OPT	  (1 << 3)	/* MAC-address */
#define NDS_LANCC_OPT	  (1 << 4)	/* LAN CC-specification */
#define NDS_X121_OPT	  (1 << 5)	/* X.121-address */
#define NDS_X121_IP_OPT	  (1 << 6)	/* X.121 local international prefix */
#define NDS_X121_DCC_OPT  (1 << 7)	/* X.121 local data country code */
#define NDS_X121_DNIC_OPT (1 << 8)	/* X.121. local data network ID code */
#define NDS_E163_OPT	  (1 << 9)	/* E.163-number */
#define NDS_E163_IP_OPT	  (1 << 10)	/* E.163 local international prefix */
#define NDS_E163_CC_OPT	  (1 << 11)	/* E.163 local country code */
#define NDS_E163_NP_OPT	  (1 << 12)	/* E.163 local national prefix */
#define NDS_E163_AC_OPT	  (1 << 13)	/* E.163 local area code */
#define NDS_E164_OPT	  (1 << 14)	/* E.164-number */
#define NDS_E164_IP_OPT	  (1 << 15)	/* E.164 local international prefix */
#define NDS_E164_CC_OPT	  (1 << 16)	/* E.164 local country code */
#define NDS_E164_NDC_OPT  (1 << 17)	/* E.164 local national dest. code */
#define NDS_PVC_OPT	  (1 << 18)	/* PVC-number */
#define NDS_X21_OPT	  (1 << 19)	/* X.21-number */
#define NDS_TPI_OPT	  (1 << 20)	/* transport protocol-ID */
#define NDS_TPC_OPT	  (1 << 21)	/* transport protocol class */
#define NDS_WANCC_OPT	  (1 << 22)	/* WAN CC-specification */
#define NDS_REG_OPT	  (1 << 23)	/* region number */
#define NDS_PROC_OPT	  (1 << 24)	/* processor number */
#define NDS_UNI_GRP_OPT	  (1 << 25)	/* unique/group ID */
#define NDS_HOST_OPT	  (1 << 26)	/* host name */
#define NDS_SNID_OPT	  (1 << 27)	/* subnet-ID */


#define NDS_ALIGN4(x)	((((x) - 1) & ~3) + 4)

						/* internal option format */
						/* structures */
#define OPTFLDTYPE	unsigned32 
#define OFFSETTYPE	unsigned16
typedef struct {
    OPTFLDTYPE		nds_opt_types ;		/* option types */
    OFFSETTYPE		nds_opt_off[1] ;	/* offset to next option */
} Nds_options ;

typedef struct {
    byte		nds_opt_rep ;		/* representation of option */
    char		nds_opt_strval[1] ;	/* option value */
} Nds_opt_str ;

typedef struct {
    byte		nds_opt_rep ;		/* representation of option */
    byte		nds_opt_roff ;		/* offset to next rec. value */
    char		nds_opt_restrval[1] ;	/* option value */
} Nds_opt_recstr ;

typedef struct {
    byte		nds_opt_rep ;		/* representation of option */
    byte		nds_opt_len ;		/* option value size */
    byte		nds_opt_octval[1] ;	/* option value */
} Nds_opt_oct ;

typedef struct {
    byte		nds_opt_rep ;		/* representation of option */
    byte		nds_opt_roff ;		/* offset to next rec. value */
    byte		nds_opt_len ;		/* option value size */
    byte		nds_opt_reoctval[1] ;	/* option value */
} Nds_opt_recoct ;

typedef struct {
    byte		nds_opt_rep ;		/* representation of option */
    signed16		nds_opt_shortval ;	/* option value */
} Nds_opt_short ;

typedef struct {
    byte		nds_opt_rep ;		/* representation of option */
    byte		nds_opt_roff ;		/* offset to next rec. value */
    signed16		nds_opt_reshortval ;	/* option value */
} Nds_opt_recshort ;

typedef struct {
    byte		nds_opt_rep ;		/* representation of option */
    signed32		nds_opt_longval ;	/* option value */
} Nds_opt_long ;

typedef struct {
    byte		nds_opt_rep ;		/* representation of option */
    byte		nds_opt_roff ;		/* offset to next rec. value */
    long		nds_opt_relongval ;	/* option value */
} Nds_opt_reclong ;

#define NDS_MX_IF	3
#define NDS_MX_PROVIDER	16
#define NDS_MX_PROFILE	32
#define NDS_MX_NETADDR	512
#define NDS_MX_OPT	(sizeof(OPTFLDTYPE)*8)

typedef struct {
    OPTFLDTYPE	nds_opttype ;
    signed32	nds_optlen ;
    void	*nds_optinfo ;
} Nds_optdesc ;


/* ----------------- FUNCTION PROTOTYPING ---------------------------------- */

void nds_store_title (char *) ;
void nds_start_ifsection (void) ;
void nds_end_ifsection (void) ;
void nds_start_if (signed32, char *) ;
void nds_end_if (char *) ;
void nds_start_prof (signed32) ;
void nds_end_prof (signed32) ;
void nds_save_cominfo (signed32, int, char *) ;
void nds_save_netaddr (signed32, char *) ;
void nds_save_option (OPTFLDTYPE, signed32, char *) ;
signed32 nds_regcomp (char *, byte **, signed32 *) ;
signed32 nds_regex (byte *, char *) ;
signed32 nds_lexdebug (signed32, signed32) ;
signed32 nds_get_option (OPTFLDTYPE, Nds_options *, void **) ;
signed32 nds_get_imp_val (T_adrdesc *, Nds_opt_short *, void *, signed32 *) ;
signed32 nds_check_tsap (T_adrdesc *, boolean) ;
boolean nds_init_match_nsap (signed32, Nds_shmruledesc **) ;
boolean nds_match_nsap (T_adrdesc *, signed32, Nds_shmifdesc **, Nds_shmrule **,
								Nds_options *) ;
boolean nds_select_nsap (T_adrdesc *, T_adrdesc	*, T_adrdesc **, T_adrdesc **,
				   signed32, Nds_shmifdesc **, Nds_shmrule **) ;
signed32 nds_map_ositoxti (signed32, struct netbuf *, struct netbuf *,
						 Nds_options *, Nds_options *) ;
signed32 nds_map_ositosock (T_adrdesc *, struct sockaddr_in *, Nds_options *) ;
signed32 nds_shmattach (void) ;
char *nds_line (void) ;


#ifdef DEBUGLEX
#define RETDBG(x)	nds_lexdebug (x, __LINE__)
#else
#define RETDBG(x)	x
#endif

#endif /* _NDSCFG_H */
