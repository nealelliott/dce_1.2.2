#!/bin/sh
#
#  @OSF_COPYRIGHT@
#  COPYRIGHT NOTICE
#  Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
#  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
#  src directory for the full copyright text.
#
#
# HISTORY
# $Log: .profile.sh,v $
# Revision 1.1.56.2  1996/03/09  23:12:22  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:34  marty]
#
# Revision 1.1.56.1  1995/12/08  15:05:29  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:46:55  root]
# 
# Revision 1.1.54.4  1994/10/05  17:51:41  keutel
# 	again
# 	[1994/10/05  17:50:06  keutel]
# 
# Revision 1.1.54.3  1994/10/05  15:38:55  keutel
# 	OT 12481: multiple routing
# 	[1994/10/05  14:35:26  keutel]
# 
# Revision 1.1.54.2  1994/05/10  15:48:29  marrek
# 	Bug fix for April 1994 submission.
# 	[1994/05/05  12:26:07  marrek]
# 
# 	Bug fixes for April 1994 submission.
# 	[1994/05/02  13:23:48  marrek]
# 
# 	April 1994 code submission.
# 	[1994/04/27  12:06:58  marrek]
# 
# Revision 1.1.54.1  1994/02/22  15:19:51  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  08:49:55  marrek]
# 
# Revision 1.1.52.2  1993/10/14  15:56:07  keutel
# 	October 1993 code drop
# 	[1993/10/14  15:52:52  keutel]
# 
# Revision 1.1.52.1  1993/10/13  17:28:20  keutel
# 	October 1993 code drop
# 	[1993/10/09  18:45:15  keutel]
# 
# Revision 1.1.4.5  1992/12/31  16:33:33  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  12:50:55  bbelch]
# 
# Revision 1.1.4.4  1992/12/07  13:18:49  zeliff
# 	Removed duplicate OSF copyright markers
# 	[1992/12/06  00:42:36  zeliff]
# 
# Revision 1.1.4.3  1992/11/27  13:06:59  marrek
# 	November 1992 code drop
# 	[1992/11/26  08:32:54  marrek]
# 
# Revision 1.1.4.2  1992/10/05  16:34:39  marrek
# 	Insert #!/bin/sh .
# 	[1992/10/05  16:34:09  marrek]
# 
# Revision 1.1.2.3  1992/06/01  20:13:19  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  18:12:27  zeliff]
# 
# Revision 1.1.2.2  1992/01/21  23:31:06  weisman
# 		Copied from dce.75.
# 	[1992/01/21  23:30:49  weisman]
# 
# $EndLog$
#
#
# $RCSfile: .profile.sh,v $ $Revision: 1.1.56.2 $ $Date: 1996/03/09 23:12:22 $
#
D2_CL=/opt/dcelocal/var/adm/directory/gds
D2_SV=/opt/dcelocal/var/directory/gds/adm

D2_USER=root; export D2_USER

D2_LOG_DIR=$D2_CL/dua; export D2_LOG_DIR
D2_LOG=off; export D2_LOG

if [ $D2_LOG = "on" ]
then
    SVC_CACHE_ROUTING="-P$D2_CL/cache/ \
	-vgds:general.1,ipc.2:BINFILE.2.2000:LOG%d \
	-wFATAL:TEXTFILE.1.100:EXC%d;GOESTO:gds  -wERROR:GOESTO:FATAL \
	-wNOTICE:GOESTO:FATAL -wWARNING:GOESTO:FATAL \
	-wNOTICE_VERBOSE:GOESTO:FATAL"
    SVC_CSTUB_ROUTING="-P$D2_CL/cstub/ \
	-vgds:general.1,asn1.2,ipc.2,ros.1,cmx.1:BINFILE.2.2000:LOG%d \
	-wFATAL:TEXTFILE.1.100:EXC%d;GOESTO:gds  -wERROR:GOESTO:FATAL \
	-wNOTICE:GOESTO:FATAL -wWARNING:GOESTO:FATAL \
	-wNOTICE_VERBOSE:GOESTO:FATAL"
    SVC_MONIT_ROUTING="-P$D2_CL/adm/ \
	-vgds:general.1,ipc.2:BINFILE.2.2000:LOG%d \
	-wFATAL:TEXTFILE.1.100:EXC%d;GOESTO:gds  -wERROR:GOESTO:FATAL \
	-wNOTICE:GOESTO:FATAL -wWARNING:GOESTO:FATAL \
	-wNOTICE_VERBOSE:GOESTO:FATAL"
    SVC_SSTUB_ROUTING="-P$D2_SV/sstub/ \
	-vgds:general.1,ipc.2,ros.1,cmx.1:BINFILE.2.2000:LOG%d \
	-wFATAL:TEXTFILE.1.100:EXC%d;GOESTO:gds  -wERROR:GOESTO:FATAL \
	-wNOTICE:GOESTO:FATAL -wWARNING:GOESTO:FATAL \
	-wNOTICE_VERBOSE:GOESTO:FATAL"
    SVC_DSA_ROUTING="-P$D2_SV/dsa/dir\${DIRID}/ \
	-vgds:general.2,asn1.2,apdu.1,ipc.2:BINFILE.2.2000:LOG%d \
	-wFATAL:TEXTFILE.1.100:EXC%d;GOESTO:gds  -wERROR:GOESTO:FATAL \
	-wNOTICE:GOESTO:FATAL -wWARNING:GOESTO:FATAL \
	-wNOTICE_VERBOSE:GOESTO:FATAL"
    SVC_UPDAT_ROUTING="-P$D2_SV/dsa/dir\${DIRID}/update/ \
	-vgds:general.1,apdu.1,ipc.2:BINFILE.2.2000:LOG%d \
	-wFATAL:TEXTFILE.1.100:EXC%d;GOESTO:gds  -wERROR:GOESTO:FATAL \
	-wNOTICE:GOESTO:FATAL -wWARNING:GOESTO:FATAL \
	-wNOTICE_VERBOSE:GOESTO:FATAL"
    SVC_GDS_DBG="gds:general.2,apdu.1,ipc.2:BINFILE.5.1000:$D2_LOG_DIR/log%d"
    export SVC_GDS_DBG
else
    SVC_CACHE_ROUTING="-P$D2_CL/cache/ \
	-vgds:*.0:BINFILE.2.2000:LOG%d -wFATAL:TEXTFILE.1.100:EXC%d;GOESTO:gds \
	-wERROR:DISCARD:- -wNOTICE:DISCARD:- -wNOTICE_VERBOSE:DISCARD:- \
	-wWARNING:DISCARD:-"
    SVC_CSTUB_ROUTING="-P$D2_CL/cstub/ \
	-vgds:*.0:BINFILE.2.2000:LOG%d -wFATAL:TEXTFILE.1.100:EXC%d;GOESTO:gds \
	-wERROR:DISCARD:- -wNOTICE:DISCARD:- -wNOTICE_VERBOSE:DISCARD:- \
	-wWARNING:DISCARD:-"
    SVC_MONIT_ROUTING="-P$D2_CL/adm/ \
	-vgds:*.0:BINFILE.2.2000:LOG%d -wFATAL:TEXTFILE.1.100:EXC%d;GOESTO:gds \
	-wERROR:DISCARD:- -wNOTICE:DISCARD:- -wNOTICE_VERBOSE:DISCARD:- \
	-wWARNING:DISCARD:-"
    SVC_SSTUB_ROUTING="-P$D2_SV/sstub/ \
	-vgds:*.0:BINFILE.2.2000:LOG%d -wFATAL:TEXTFILE.1.100:EXC%d;GOESTO:gds \
	-wERROR:DISCARD:- -wNOTICE:DISCARD:- -wNOTICE_VERBOSE:DISCARD:- \
	-wWARNING:DISCARD:-"
    SVC_DSA_ROUTING="-P$D2_SV/dsa/dir\${DIRID}/ \
	-vgds:*.0:BINFILE.2.2000:LOG%d -wFATAL:TEXTFILE.1.100:EXC%d;GOESTO:gds \
	-wERROR:DISCARD:- -wNOTICE:DISCARD:- -wNOTICE_VERBOSE:DISCARD:- \
	-wWARNING:DISCARD:-"
    SVC_UPDAT_ROUTING="-P$D2_SV/dsa/dir\${DIRID}/update/ \
	-vgds:*.0:BINFILE.2.2000:LOG%d -wFATAL:TEXTFILE.1.100:EXC%d;GOESTO:gds \
	-wERROR:DISCARD:- -wNOTICE:DISCARD:- -wNOTICE_VERBOSE:DISCARD:- \
	-wWARNING:DISCARD:-"
fi
export SVC_CACHE_ROUTING SVC_CSTUB_ROUTING SVC_MONIT_ROUTING
export SVC_SSTUB_ROUTING SVC_DSA_ROUTING SVC_UPDAT_ROUTING

CMXINIT=-f; export CMXINIT
CSTUB_TSTRACE="-SDp0d256"; export CSTUB_TSTRACE
SSTUB_TSTRACE="-SDp0d256"; export SSTUB_TSTRACE
umask 0
