/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
# ifndef RCS_agt_sysid_h_Included
# define RCS_agt_sysid_h_Included
 static char *RCSID_agt_sysid_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_sysid.h,v 1.1.2.2 1996/03/11 14:06:21 marty Exp $";
# endif /* Not RCS_agt_sysid_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_sysid.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:21  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:03  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:20:58  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:20:38  dce-sdc]
 * 
 * Revision 1.1.2.4  96/01/12  06:18:45  06:18:45  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  06:18:33  kishi]
 * 
 * Revision 1.1.2.3  1995/10/26  02:47:52  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:47:45  kishi]
 * 
 * Revision 1.1.2.2  1995/10/20  05:11:08  ichikawa
 * 	Add #define SECACLSITECLOSE 35.
 * 	[1995/10/20  05:10:50  ichikawa]
 * 
 * Revision 1.1.2.1  1995/09/09  07:37:06  kishi
 * 	Initial revision
 * 	[1995/09/09  07:22:23  kishi]
 * 
 * $EndLog$
 */
#define DOPEN		1
#define DFCNTL		2
#define DCLOSE		3
#define DCREAT		4
#define DSTAT		5
#define DREMOVE		6
#define DREAD		7
#define DWRITE		8
#define DMKDIR		9
#define DACCESS		10
#define DRENAME		11
#define DOPENDIR        12
#define DCLOSEDIR       13
#define SECPGOUNTI	14
#define SECIDGENNAME	15
#define DREADDIR	16
#define DUTIME		17
#define SECIDPARCENAME	18
#define SECIDTOUNIXNUM	19
#define DCHOWN		20
#define SECUNIXNUMTOID	21
#define SECGETCELLNAME  22
#define SECSITEOPEN	23
#define DAFSSYSCALL	24
#define SECIDPARCEGROUP 25
#define DPIOCTL		26
#define SECIDGENGROUP   27
#define DCHMOD		28
#define DTIME		29
#define DSYSLOG		30
#define DLSEEK		31
#define DTRUNCATE	32
#define SECIDPARSENAME  33
#define SECACLREPLACE   34
#define SECACLSITECLOSE 35

