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
# $Log: gdsdeact.sh,v $
# Revision 1.1.10.2  1996/03/09  23:13:01  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:12  marty]
#
# Revision 1.1.10.1  1995/12/08  15:25:43  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1994/08/30  16:21 UTC  dtruong
# 	Replace kill DUA-Cache with signal 15
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  18:24 UTC  dtruong
# 	Merge to DCE 1.1
# 
# 	HP revision /main/HPDCE01/1  1994/04/18  23:11 UTC  dtruong
# 	Port to HPUX 10.0
# 	[1995/12/08  14:54:04  root]
# 
# Revision 1.1.6.2  1993/08/11  14:20:48  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:10:55  marrek]
# 
# Revision 1.1.4.3  1992/12/31  18:39:18  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:11:55  bbelch]
# 
# Revision 1.1.4.2  1992/09/21  14:48:28  marrek
# 	Corrections for bug 5206.
# 	[1992/09/16  15:14:31  marrek]
# 
# Revision 1.1.2.2  1992/06/01  20:11:46  melman
# 	New GDS merged drop
# 	[1992/05/28  21:04:36  melman]
# 
# $EndLog$
#
# $Id: gdsdeact.sh,v 1.1.10.2 1996/03/09 23:13:01 marty Exp $
#
#****************************************************************************
#                                                                           *
#         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
#                                ALL RIGHTS RESERVED                        *
#                                                                           *
#****************************************************************************

# Shell-script used to deactivate a directory system
# Input Parameter
#            $1 = Path name <dcelocal>/etc
# Exit-values:
#            0  = Operation successfully performed
#            1  = Error: Function can't be executed (processes are
#		         still running)
#            4  = Error: No configuration information available
#            5  = Error: Directory system isn't active
#           10  = Error: The directory system is still in use
#

TIMEOUT=60
CL_PATH=`dirname $1`/var/adm/directory/gds

# Verify for an existing directory system configuration file
# ----------------------------------------------------------
#
if [ -s $1/gdsconfig ]
then
	# Verify for the existence of directory system user and
	# -----------------------------------------------------
        # deactivate all available directory processes
        # --------------------------------------------
        # (Consistency of the data base- resp. cache-files is regarded)

	DELCMD=`gdsdirinfo | awk 'BEGIN {NOUSRPROC=0;NOPROC1=0;NOPROC2=0}
			$1 ~ /Monitor|DSA/ { PID1[++NOPROC1] = $2 }
			$1 ~ /DUA-Cache|C-Stub|S-Stub/ { PID2[++NOPROC2] = $2 }
			$1 == "Dir-User" { for (i = 1; i <= NOPROC2; i++ )
						if ( PID2[i] == $2 ) break ;
				           if ( i > NOPROC2 )	NOUSRPROC++ }
			END {
			printf "if [ %d -gt 0 ] ; then exit 10 ; fi ;\n", NOUSRPROC
			printf "if [ %d -eq 0 -a %d -eq 0 ] ; then exit 5 ; fi ;\n", NOPROC1, NOPROC2
			if (NOPROC1 > 0) {
				printf "gdsutil -k 15 "
				for (I = 1; I <= NOPROC1; I++)
					printf "%d ", PID1[I]
			}
			if (NOPROC2 > 0) {
				printf ";\ngdsutil -k 9 "
				for (I = 1; I <= NOPROC2; I++)
					printf "%d ", PID2[I]
			}
			}'`
	echo ">>: delinfo <<<\n$DELCMD>>>" >&2
	if eval $DELCMD ; then : ; else exit $? ; fi

	# cancel crontab entries
	# ----------------------
	if [ -s $CL_PATH/.crontab ]
	then
		crontab -l|sort > $CL_PATH/.crontab1.$$
		cat $CL_PATH/.crontab|sort > $CL_PATH/.crontab2.$$
		comm -23 $CL_PATH/.crontab1.$$ $CL_PATH/.crontab2.$$ > \
							$CL_PATH/.crontab3.$$
		if [ -s $CL_PATH/.crontab3.$$ ]
		then
	        	crontab $CL_PATH/.crontab3.$$
		else
			crontab -r
		fi
		rm $CL_PATH/.crontab[1-3].$$
	        echo ">>: crontab entries cancelled" >&2
	fi

	# wait for termination of all processes
	# -------------------------------------
	while [ $TIMEOUT -gt 0 -a `gdsdirinfo|wc -l` -gt 2 ]
	do
		echo ">>: waiting for process termination (timeout = $TIMEOUT sec)" >&2
		TIMEOUT=`expr $TIMEOUT - 2`
		sleep 2
	done
	[ $TIMEOUT -le 0 ] && exit 1

	# remove IPC-resources from system
	# --------------------------------
	gdsipcinit -R
	exit $?
else
	# Error: no configuration information available
        exit 4
fi
