#ifdef HPUX1000P
#!/usr/bin/sh
#else
#!/bin/sh
#endif
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: gdslog.sh,v $
# Revision 1.1.10.2  1996/03/09  23:13:06  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:17  marty]
#
# Revision 1.1.10.1  1995/12/08  15:25:57  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  18:33 UTC  dtruong
# 	Merge to DCE 1.1
# 
# 	HP revision /main/HPDCE01/1  1994/04/19  23:15 UTC  dtruong
# 	Port to HPUX 10.0
# 
# 	HP revision /main/dtruong_mothra/1  1994/04/19  23:13 UTC  dtruong
# 	Port to HPUX 10.0
# 	[1995/12/08  14:54:08  root]
# 
# Revision 1.1.6.2  1993/08/11  14:24:39  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:11:29  marrek]
# 
# Revision 1.1.4.3  1992/12/31  18:39:26  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:12:08  bbelch]
# 
# Revision 1.1.4.2  1992/10/05  16:44:18  marrek
# 	Insert #!/bin/sh .
# 	[1992/10/05  16:41:32  marrek]
# 
# Revision 1.1.2.2  1992/06/01  20:12:12  melman
# 	New GDS merged drop
# 	[1992/05/28  21:04:54  melman]
# 
# $EndLog$
#
# $Id: gdslog.sh,v 1.1.10.2 1996/03/09 23:13:06 marty Exp $
#
#****************************************************************************
#                                                                           *
#         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
#                                ALL RIGHTS RESERVED                        *
#                                                                           *
#****************************************************************************

# Shell-script used to switch the logging system of the directory system
# to an off-state resp. on-state
# Input parameter:
#                $1 = Path name <dcelocal>/var/adm/directory/gds
#                $2 = New state of the logging system -> on | off
# Exit values:
#		0 = Operation successfully performed
#		8 = Can't perform distributed command
#

TIMEOUT=30
DISLOG=6
ENALOG=7
DCMD=$ENALOG
CACHESID=1
CSTUBSID=2
MONITORID=5
SSTUBSID=10
DSASID=30

# change the logging environment variable within the '.profile'-file
# ------------------------------------------------------------------

if [ "$2" != "$D2_LOG" ]
then
    D2LOG="D2_LOG=$2; export D2_LOG"

    awk -F= ' $1 == "D2_LOG"   { print P1 ; next } 
			   { print $0 }
	  ' P1="$D2LOG" $1/.profile > $1/.profile.$$ || exit $?
    mv $1/.profile.$$ $1/.profile || exit $?
    chmod 755 $1/.profile || exit $?

    # get new environment
    # -------------------
    . $1/.profile
fi

# Verify for an active directory system
# ---------------------------------------

eval `gdsdirinfo | awk 'BEGIN { NOPROC=0; CAIPCID=0; MOIPCID=0; CSIPCID=0; \
				       for (i=11;i<=50;i++) SVIPCID[i]=0 }
			$1 == "DUA-Cache" { NOPROC++; CAIPCID=$4 }
			$1 == "Monitor"   { NOPROC++; MOIPCID=$4 }
			$1 == "C-Stub"    { NOPROC++; CSIPCID=$4 }
			$1 == "S-Stub"    { NOPROC++; SVIPCID[$4]=$4 }
			$1 == "DSA"       { NOPROC++; SVIPCID[$4]=$4 }
			END { printf "NOPROC=%d; IPCIDS=\"%d %d %d", \
				NOPROC, CAIPCID, MOIPCID, CSIPCID
				for (i=11;i<=50;i++)
					if (SVIPCID[i] > 0)
						printf " %d", SVIPCID[i]
				printf "\"\n" }'`
echo ">>: procinfo (NOPROC: $NOPROC IPCIDS: $IPCIDS)" >&2

# Send distributed log-command to all directory processes
# -------------------------------------------------------
if [ $NOPROC -gt 0 ]
then
    [ "$2" = "off" ] && DCMD=$DISLOG

    for IPCID in $IPCIDS
    do
	[ $IPCID = $CACHESID ] && {
	    gdsdistcmd -c$DCMD -s$IPCID -t$TIMEOUT -p"$SVC_CACHE_ROUTING" >&2 \
							|| exit 8 ; continue ; }
	[ $IPCID = $CSTUBSID ] && {
	    gdsdistcmd -c$DCMD -s$IPCID -t$TIMEOUT -p"$SVC_CSTUB_ROUTING" >&2 \
							|| exit 8 ; continue ; }
	[ $IPCID = $MONITORID ] && {
	    gdsdistcmd -c$DCMD -s$IPCID -t$TIMEOUT -p"$SVC_MONIT_ROUTING" >&2 \
							|| exit 8 ; continue ; }
	[ $IPCID -gt $SSTUBSID -a $IPCID -le $DSASID ] && {
	    gdsdistcmd -c$DCMD -s$IPCID -t$TIMEOUT -p"$SVC_SSTUB_ROUTING" >&2 \
							|| exit 8 ; continue ; }
	[ $IPCID -gt $DSASID ] && {
	    DIRID=`expr $IPCID - $DSASID`
	    eval SVC_DSA_INFO=\"$SVC_DSA_ROUTING\"	
	    gdsdistcmd -c$DCMD -s$IPCID -t$TIMEOUT -p"$SVC_DSA_INFO" >&2 \
								   || exit 8 ; }
    done
fi
exit 0
