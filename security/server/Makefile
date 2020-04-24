# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.8.2  1996/03/09  23:29:56  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:17  marty]
#
# Revision 1.1.8.1  1995/12/08  17:46:29  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:24 UTC  psn
# 	Add pwd_mgmt to SUBDIR list so that pwd_strengthd gets exported
# 
# 	HP revision /main/HPDCE02/1  1994/12/22  22:12 UTC  aha
# 	HP version of Makefile
# 
# 	HP revision /main/aha_mothra1/1  1994/12/22  22:08 UTC  aha
# 	Do not build pwd_mgmt (now in /dce/src/HP/security/server/pwd_mgmt)
# 	[1995/12/08  17:19:11  root]
# 
# Revision 1.1.6.4  1994/10/05  18:28:58  annie
# 	CR11488 - moved sec_clientd to src/nosupport, d so o remove it from here
# 	[1994/10/05  18:28:30  annie]
# 
# Revision 1.1.6.3  1994/09/16  16:03:53  rsalz
# 	Don't build sec_clientd (OT CR 11488).
# 	[1994/09/15  19:39:51  rsalz]
# 
# Revision 1.1.6.2  1994/07/15  15:00:34  mdf
# 	Hewlett Packard Security Code Drop
# 	[1994/07/14  17:18:00  mdf]
# 
# Revision 1.1.6.1  1994/05/11  19:25:37  ahop
# 	hp_sec_to_osf_2 drop
# 	Add subdirectory pwd_mgmt.
# 	[1994/04/29  21:23:00  ahop]
# 
# Revision 1.1.4.4  1993/03/05  18:30:55  frisco
# 	add sec_salvage_db
# 	[1993/03/03  20:05:10  frisco]
# 
# Revision 1.1.4.3  1992/12/29  16:33:29  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  21:21:22  zeliff]
# 
# Revision 1.1.4.2  1992/08/31  18:22:13  sekhar
# 	First security replication code drop
# 	[1992/08/31  15:09:17  sekhar]
# 
# Revision 1.1.2.2  1992/04/07  19:19:11  mckeen
# 	Commented out DATA_SUBDIRS
# 	[1992/03/13  15:46:52  mckeen]
# 
# Revision 1.1  1992/01/19  14:45:03  devrcs
# 	Initial revision
# 
# $EndLog$
#

#
# Copyright (c) Hewlett-Packard Company 1991, 1994
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /server Makefile
#

# Subdirectories that export files
# (or build their own header files in this pass)
EXPINC_SUBDIRS = rs rrs lm

# Subdirectories that have DATAFILES
#DATA_SUBDIRS = rs 

# Sudirectories that export libraries
EXPLIB_SUBDIRS = rs rrs rsdb lm

# Subdirectories that build programs
SUBDIRS	= bin sec_salvage_db pwd_mgmt

.include <${RULES_MK}>
