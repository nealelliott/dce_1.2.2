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
# $Log: gdsstart.sh,v $
# Revision 1.1.10.2  1996/03/09  23:13:07  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:18  marty]
#
# Revision 1.1.10.1  1995/12/08  15:26:02  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1994/10/10  15:25 UTC  dtruong
# 	Merge onto Hpdce02_04
# 	[1995/12/08  14:54:08  root]
# 
# Revision 1.1.8.6  1994/10/05  15:38:56  keutel
# 	OT 12481: multiple routing
# 	[1994/10/05  14:35:28  keutel]
# 
# Revision 1.1.8.5  1994/07/06  15:06:45  marrek
# 	July 1994 code submission.
# 	[1994/07/04  13:12:23  marrek]
# 
# Revision 1.1.8.4  1994/06/21  14:44:45  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:28:58  marrek]
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  18:35 UTC  dtruong
# 	Merge to DCE 1.1
# 
# 	HP revision /main/HPDCE01/1  1994/04/19  23:21 UTC  dtruong
# 	Port to HPUX 10.0
# 
# Revision 1.1.6.2  1993/08/11  14:25:41  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:11:38  marrek]
# 
# Revision 1.1.4.5  1992/12/31  18:39:28  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:12:11  bbelch]
# 
# Revision 1.1.4.4  1992/12/04  18:03:34  marrek
# 	November 1992 code drop
# 	remove CMX-Logfiles from /opt/dcelocal/var/adm/directory/gds/cstub
# 	and /opt/dcelocal/var/directory/gds/adm/sstub
# 	[1992/12/04  10:47:09  marrek]
# 
# Revision 1.1.4.3  1992/11/27  17:01:23  marrek
# 	November 1992 code drop
# 	[1992/11/26  10:09:22  marrek]
# 
# Revision 1.1.4.2  1992/10/05  16:44:27  marrek
# 	Insert #!/bin/sh .
# 	[1992/10/05  16:41:41  marrek]
# 
# Revision 1.1.2.2  1992/06/01  20:12:18  melman
# 	New GDS merged drop
# 	[1992/05/28  21:04:59  melman]
# 
# $EndLog$
#
# $RCSfile: gdsstart.sh,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:13:07 $
#
#****************************************************************************
#                                                                           *
#         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
#                                ALL RIGHTS RESERVED                        *
#                                                                           *
#****************************************************************************

# Shell-script used to activate a directory system
# The directory system is activated by using the informations contained
# in the configuration file 'd2config'. An entry within this file has the
# following format:
#
# DIR-ID:<1-20>
#  CONF-TYPE:<Client-System | Server-System | Clt/Srv-System | Clt/Srv-System/L>
#    SERVER-NO:<1-64>
#      USEREXIT:<- | *>
#        UPDATE:<- | *>
#          DB-TYPE:<- | C-isam | informix>
#            CLIENTS:<1-128>
#
# Input parameter:
#                $1 = Path name <dcelocal>/var/adm/directory/gds
#                $2 = Path name <dcelocal>/etc
#                $3 = Path name <dcelocal>/var/directory/gds
#
# Exit values:
#                0 = Operation successfully performed
#                3 = Error: directory system is already active
#                4 = Error: No configuration information available
#

# ----------------------------------------------------------------------------

handle_error() {
# Short description: This function handles the errors which are returned from
#		     the started processes.
# Input parameter: $1 = process which was started
#			(1 = gdsipcinit, 2 = gdsipcchk, 3 = gdscache,
#			 4 = gdscstub, 5 = gdssstub, 6 = gdsdsa)
#                  $2 = Exit-Value of the started process
# Return values:   0  = Function successfully performed
#		   1  = Can't get parameter value

    if [ "$1" -ge 32 ]
    then
	EXIT_VAL=$2
    else
	if [ "$1" -ne 0 ]
	then
		EXIT_VAL=2
	fi
    fi

    case $1 in
    1)	# gdsipcinit
	case $EXIT_VAL in 
	*) # system error
	   exit `expr 32 + $EXIT_VAL`
	esac;;
    2)	# gdsipcchk
	case $EXIT_VAL in 
	*) # system error
	   exit `expr 64 + $EXIT_VAL`
	esac;;
    3)	# gdscache
	case $EXIT_VAL in 
	*) # system error
	   exit `expr 64 + 32 + $EXIT_VAL`
	esac;;
    4)	# gdscstub
	case $EXIT_VAL in 
	*) # system error
	   exit `expr 128 + $EXIT_VAL`
	esac;;
    5)	# gdssstub
	case $EXIT_VAL in 
	*) # system error
	   exit `expr 128 + 32 + $EXIT_VAL`
	esac;;
    6)	# gdsdsa
	case $EXIT_VAL in 
	*) # system error
	   exit `expr 128 + 64 + $EXIT_VAL`
	esac;;
    *)	# unknown process
	exit 2
    esac
}

# ----------------------------------------------------------------------------

get_dirparam() {
# Short description: This function gets the value which is assigned to the given
#		     parameter (name) from the directory system parameter file.
# Input parameter: $1 = Parameter name
#                  $2 = Path name
#                  $3 = blank shall be appended to output (yes/no)
# Return values:   0  = Function successfully performed
#		   1  = Can't get parameter value
	case $3 in
	no )  # don't append blank to output
	      awk '$1 == "'$1'" { for (i = 2; i < NF && index ($(i), "#") != 1; i++)
				printf "%s", $(i)
			        exit
			  }' $2/conf/dirparam || return 1;;
	yes ) # append blank to output
	      awk '$1 == "'$1'" { for (i = 2; i < NF && index ($(i), "#") != 1; i++)
				printf "%s ", $(i)
			        exit
			  }' $2/conf/dirparam || return 1;;
	* )   # wrong input
	      return 1;;
	esac

	return 0
}

# ----------------------------------------------------------------------------

# main () {

DUADIR=dua
ADMDIR=adm
DSADIR=dsa/dir
CSTUBDIR=cstub
SSTUBDIR=sstub
CACHEDIR=cache
IPCDIR=ipc
UPDATEDIR=update
EXCFILES="EXC*"
TRCFILES="LOG*"

CLTSYS=Client-System
CLTSRVSYS=Clt/Srv-System
UPDYES="*"
IPCPROC=false
CACHEPROC=false
CSTUBPROC=false
PREDIRID=1
DBINFORMIX="informix"
DBCISAM="C-isam"
LOADF="-s60"
USRMODE="-u"
LOCMODE=
RESULT=4

# get environment informations
. $1/.profile

# Verify if directory system is already active
echo "d20start: IPC-state informations:"
gdsdirinfo && {
	echo "d20start: ERROR - directory system is already active" ; exit 3 ; }

# get and evaluate configuration information
exec < $2/gdsconfig
while read FNAME DIRID FNAME CONFTYPE FNAME NOSERVER FNAME UPDATE FNAME DBTYPE FNAME CLIENTS FNAME AUTHMECH
do
	RESULT=0
	if [ $IPCPROC = "false" ]
	then
		# remove logging-information
		echo	$3/$ADMDIR/$DSADIR[1-9]/$TRCFILES \
		echo	$3/$ADMDIR/$DSADIR[1-9]/$EXCFILES \
			$3/$ADMDIR/$DSADIR[1-2][0-9]/$TRCFILES \
			$3/$ADMDIR/$DSADIR[1-2][0-9]/$EXCFILES \
			$1/$CSTUBDIR/$TRCFILES $1/$CSTUBDIR/$EXCFILES \
			$3/$ADMDIR/$SSTUBDIR/$TRCFILES \
			$3/$ADMDIR/$SSTUBDIR/$EXCFILES \
 			$1/$DUADIR/$TRCFILES $1/$DUADIR/$EXCFILES \
			$1/$CACHEDIR/$TRCFILES $1/$CACHEDIR/$EXCFILES \
			$1/$ADMDIR/$TRCFILES $1/$ADMDIR/$EXCFILES \
			$1/$IPCDIR/* \
			$3/$ADMDIR/$DSADIR[1-2][0-9]/$UPDATEDIR/$TRCFILES \
			$3/$ADMDIR/$DSADIR[1-2][0-9]/$UPDATEDIR/$EXCFILES \
			$3/$ADMDIR/$DSADIR[1-9]/$UPDATEDIR/$TRCFILES \
			$3/$ADMDIR/$DSADIR[1-9]/$UPDATEDIR/$EXCFILES | \
			xargs -n16 /bin/rm -f || exit $?

		# initialize IPC-resources
		gdsipcinit `cat $1/conf/ipcconf` || handle_error 1 $?
		echo ">>: gdsipcinit started"

		# activate IPC-monitoring process
		gdsipcchk $SVC_SERVERNAME $SVC_MONIT_ROUTING || handle_error 2 $?
		echo ">>: gdsipcchk started"
		IPCPROC="true"
	fi

	if [ $CACHEPROC = "false" ]
	then
		# activate DUA-cache process
		CACHE_CONF_INFO="`get_dirparam CACHE_CONF_INFO $1 yes`"
		gdscache $SVC_CACHE_ROUTING $CACHE_CONF_INFO \
							    || handle_error 3 $?
		echo ">>: gdscache $CACHE_CONF_INFO started"
		CACHEPROC="true"
	fi

	if [ $CSTUBPROC = "false" ]
	then
		# activate C-stub process
		gdscstub -d$PREDIRID $SVC_CSTUB_ROUTING || handle_error 4 $?
		echo ">>: gdscstub -d$PREDIRID started"
		CSTUBPROC="true"
	fi

	if [ $CONFTYPE = $CLTSRVSYS ]
	then
		# activate S-stub process
		gdssstub -d$DIRID $SVC_SSTUB_ROUTING || handle_error 5 $?
		echo ">>: gdssstub -d$DIRID started"
	fi

	if [ $CONFTYPE != $CLTSYS ]
	then
		# activate DSA-process[es]
		while [ $NOSERVER -gt 0 ]
		do
			if [ "$UPDATE" = "$UPDYES" ]
			then
				# enable shadow update logging
				UPDMODE="-le"
			else
				# disable shadow update logging
				UPDMODE="-ld"
			fi
			eval SVC_DSA_INFO=\"$SVC_DSA_ROUTING\"
			gdsdsa -d$DIRID $USRMODE $LOCMODE $UPDMODE $LOADF \
				-A$AUTHMECH $SVC_DSA_INFO || handle_error 6 $?
			echo ">>: gdsdsa -d$DIRID $USRMODE $LOCMODE $UPDMODE $LOADF -A$AUTHMECH started"
			NOSERVER=`expr $NOSERVER - 1`
		done

	fi
done

# activate crontab entries
if [ $RESULT -eq 0 -a -s $1/.crontab ]
then
	crontab -l > $1/.crontab1.$$
	cat $1/.crontab >> $1/.crontab1.$$
        crontab $1/.crontab1.$$
	rm $1/.crontab1.$$
        echo ">>: crontab entries activated"
fi

exit $RESULT
# }
