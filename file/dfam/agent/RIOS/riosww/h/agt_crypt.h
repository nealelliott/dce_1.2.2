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
# ifndef RCS_agt_crypt_h_Included
# define RCS_agt_crypt_h_Included
 static char *RCSID_agt_crypt_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_crypt.h,v 1.1.2.2 1996/03/11 14:05:59 marty Exp $";
# endif /* Not RCS_agt_crypt_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_crypt.h,v $
 * Revision 1.1.2.2  1996/03/11  14:05:59  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:47  marty]
 *
 * Revision 1.1.2.1  1996/02/19  10:05:18  dce-sdc
 * 	First Submit
 * 	[1996/02/19  10:04:42  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:26:13  05:26:13  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:26:04  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  01:49:03  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:48:54  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:43:04  kishi
 * 	Initial revision
 * 	[1995/09/09  07:15:11  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_CRYPT_H
#define _AGT_CRYPT_H


struct d_crypt_info{
	int 	crypttype;
	void	*nextkey;
	void 	*randval;
	void	(* encrypt)();
	void	(* decrypt)();
};


struct d_crypt_tbl{
	int	crypttype;
	void	(* encrypt)();
	void	(* decrypt)();
};

extern struct d_crypt_info 	crypt_info;
extern struct d_crypt_tbl 	crypt_tbl[];

#endif

