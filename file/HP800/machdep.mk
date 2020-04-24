# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# @HP_COPYRIGHT@
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.20.1  1996/10/02  16:56:33  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:03:13  damon]
#
# Revision 1.1.14.3  1994/07/13  22:30:50  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:12:09  devsrc]
# 
# 	Removed OSDEBUG from HP kernel flags
# 	[1994/06/27  18:59:31  mckeen]
# 
# Revision 1.1.14.2  1994/06/09  13:49:27  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:22:55  annie]
# 
# Revision 1.1.14.1  1994/02/04  20:03:43  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:05:30  devsrc]
# 
# Revision 1.1.12.2  1994/01/20  17:32:20  annie
# 	added copyright header
# 	[1994/01/20  17:32:08  annie]
# 
# Revision 1.1.12.1  1993/12/07  17:10:40  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/02  22:34:43  jaffe]
# 
# Revision 1.1.10.2  1993/07/19  19:27:47  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:21:12  zeliff]
# 
# Revision 1.1.8.3  1993/07/16  19:00:21  kissel
# 	Change kernel header file handling for HPUX.
# 	[1993/06/21  14:19:28  kissel]
# 
# Revision 1.1.6.3  1993/06/10  22:13:45  kissel
# 	Remove the check for HP site since all HPUX builds should do this.
# 	[1993/06/10  22:12:32  kissel]
# 
# 	ifdef for HP
# 	[1993/05/10  13:31:49  robinson]
# 
# 	Check "IN_SHLIB" so that don't define NO_ZFLAG
# 	in contexts that need PIC.
# 	[1993/05/04  18:25:07  robinson]
# 
# 	GAMERA Merge
# 	[1993/04/09  19:46:51  toml]
# 
# Revision 1.1.6.2  1993/06/04  13:58:08  kissel
# 	Initial HPUX RP version.
# 	[1993/06/03  20:27:54  kissel]
# 
# Revision 1.1.2.7  1993/03/15  16:46:50  toml
# 	Add NO_ZFLAG.
# 	[1993/03/15  15:59:02  toml]
# 
# Revision 1.1.2.6  1992/11/23  17:50:22  tmm
# 	tmm  11/11/92   Add -DHP_DFS to define line.
# 	[1992/11/23  17:20:04  tmm]
# 
# Revision 1.1.2.5  1992/10/26  20:19:55  toml
# 	Add STUB_OPT_LEVEL -O.
# 	[1992/10/26  20:19:24  toml]
# 
# Revision 1.1.2.4  1992/10/16  21:41:55  toml
# 	More adjustments. UX9.0 support.
# 	[1992/10/14  16:35:34  toml]
# 
# Revision 1.1.2.3  1992/06/09  21:17:42  toml
# 	Add -DKERNEL and -D_KERNEL
# 	[1992/06/09  21:17:08  toml]
# 
# Revision 1.1.2.2  1992/06/09  19:24:12  tmm
# 	06/09/92 tmm Created.
# 	[1992/06/09  19:22:46  tmm]
# 
# $EndLog$


.if !defined(TARGET_OS_VERSION)
# default version is UX9.0 ...
TARGET_OS_VERSION = UX90
.endif
.if ${TARGET_OS_VERSION} == "UX80"
KERNELCFLAGS = -DKERNEL -D_KERNEL -DGENERIC -DWSIO_PHYSIO -DEQUIVMEM -DNGRAF_WS -DSPARSE_PDIR \
		-D_WSIO -DSNAKES_IO -DWIPER -DPA89 -DGETMOUNT -DLOCAL_DISC -DFSS -DREGION -DAPPLETALK \
		-D_HPUX_SOURCE -DHFS -DFS_TUNE1 -DAUDIT -DACLS -DFSD_KI -DPSTAT -DPOSIX -DMP \
		-DLWSYSCALL -DSYMLINK -DICA_ON -DRDB -DEQUIVMEM -DSYSCALLTRACE -DNOCOMPAT -DNOPAGING \
		-DBSDJOBCTL -DIODCIO -DNIO -DQUOTA -DFPC_BUG -DNSYNC -DEISA_PDC_KLUDGE -DKERNEL_SWITCH \
		-DTEAC_FLOPPY -DAUTOCHANGER -D_PA_RISC_1_1 -DTIMEX_BUG -DTIMEX \
		-DAFS_DEBUG -DHP_DFS
.else
KERNELCFLAGS = \
		-DGENERIC -DAPPLETALK -DFDDI_VM -DHW_VISIBLE_TBL -D_UNSUPPORTED -DWSIO_PHYSIO \
		-DEQUIVMEM -DNGRAF_WS -DSPARSE_PDIR -D_WSIO -DSNAKES_IO -DPA89 -DGETMOUNT -DLOCAL_DISC -DFSS \
		-DREGION -D_HPUX_SOURCE -DHFS -DAUDIT \
		-DACLS -DFSD_KI -DPSTAT -DPOSIX -DMP -DLWSYSCALL -DICA_ON -DRDB -DEQUIVMEM -DSYSCALLTRACE \
		-DBSDJOBCTL -DQUOTA -DFPC_BUG -DNSYNC -DTEAC_FLOPPY -DAUTOCHANGER -D_PA_RISC_1_1 -DTIMEX_BUG -DTIMEX \
		-DKERNEL -D_KERNEL \
		-DAFS_DEBUG -DHP_DFS
.endif

#
# List of directories to potentially add to the INCDIRS
#
DFS_INCDIRS := ${KERNEL_CC:D${HP_KERNEL_INCDIRS}}

#
# set "arglist" for parse_ext makfile
#
parse_ext_args = action_ADDEXISTS list_KERNELCFLAGS dirs_DFS_INCDIRS addhow_APPEND prefix_-I cachetag_fl_hp800_1

# include osf.parse_ext.mk which will add any existing directories from the 
# DFS_INCDIRS list to KERNELCFLAGS
#
.include <osf.parse_ext.mk>

.if ${TARGET_OS} == "HPUX"
.if !defined(IN_SHLIB)
NO_ZFLAG = true
.endif
.else
NO_ZFLAG = true
.endif

KERNEL_LDOPTS = -R 10800 -N -e rdb_bootstrap

###KERNELCFLAGS = -DAFS_DEBUG -Wp,-H200000

###USERCFLAGS   = -Aa -DAFS_DEBUG -DAFSL_USE_RCS_ID
USERCFLAGS   = -DAFS_DEBUG ${DCEPATHS}

OS_LIBS = -lBSD

STUB_OPT_LEVEL = -O

# module specific local modifications
osi_KERNELCFLAGS =
