#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: gdsdbread.sh,v $
# Revision 1.1.11.2  1996/03/09  23:12:59  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:09  marty]
#
# Revision 1.1.11.1  1995/12/08  15:25:35  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:54:02  root]
# 
# Revision 1.1.9.2  1994/06/21  14:44:44  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:28:55  marrek]
# 
# Revision 1.1.9.1  1994/02/22  18:05:16  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:48:53  marrek]
# 
# Revision 1.1.7.2  1993/08/11  14:18:38  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:10:30  marrek]
# 
# Revision 1.1.5.6  1993/02/02  10:10:53  marrek
# 	use absolut path if filename starts with '/'
# 	[1993/02/01  17:54:42  marrek]
# 
# Revision 1.1.5.5  1992/12/31  18:39:11  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:11:47  bbelch]
# 
# Revision 1.1.5.4  1992/12/17  10:04:37  marrek
# 	Update crontab after restore
# 	[1992/12/16  13:14:51  marrek]
# 
# Revision 1.1.5.3  1992/11/27  17:01:06  marrek
# 	November 1992 code drop
# 	[1992/11/17  14:55:11  marrek]
# 
# Revision 1.1.5.2  1992/09/21  14:46:54  marrek
# 	Corrections for bug 5206.
# 	[1992/09/16  15:13:58  marrek]
# 
# Revision 1.1.2.3  1992/06/30  20:55:10  melman
# 	GDS drop of 6/16/92
# 	[1992/06/30  20:34:14  melman]
# 
# Revision 1.1.2.2  1992/06/01  20:10:14  melman
# 	New GDS merged drop
# 	[1992/05/28  21:04:26  melman]
# 
# $EndLog$
#
# $RCSfile: gdsdbread.sh,v $ $Revision: 1.1.11.2 $ $Date: 1996/03/09 23:12:59 $
#
#****************************************************************************
#                                                                           *
#         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
#                                ALL RIGHTS RESERVED                        *
#                                                                           *
#****************************************************************************

# Shell-script used to restore directory system data informations
# Input parameter:
#               $1 = Path name <dcelocal>
#		$2 = Directory system identifier
#		$3 = Command code
#		     1 = read media volume
#		     2 = stop operation
#		$4 = No. of volume from which the information should be read
#		     (if command code = 1 - irrelevant otherwise)
#		$5 = Medium (0 = floppy disk, 1 = streamer tape, 2 = file)
#		     (if command code = 1 - irrelevant otherwise)
#		$6 = User key
#		     (if command code = 1 - irrelevant otherwise)
#		$7 = Filename (from which shall be restored)
#		     (if medium = 2 - irrelevant otherwise)
# Exit values:
#		0  = Operation successfully performed
#		2  = The function can't be executed
#		6  = Can't read configuration information
#		8  = Can't send distributed command
#		11 = Can't read distribution file from media volume
#		12 = Wrong label
#		13 = Wrong user key
#		15 = Distribution file contains invalid configuration
#		     information
#		18 = Can't read data from media volume
#		19 = Can't read distribution file information
#               26 = Restored data base doesn't agree to the DSA
#               27 = Restored data base doesn't agree to the DUA-cache
#		37 = Can't read data from file
#               38 = File doesn't exist
#		39 = Can't read file list from file
#

# ----------------------------------------------------------------------------

get_dirparam() {
# Short description: This function gets the value which is assigned to the given
#		     parameter (name) from the directory system parameter file.
# Input parameter: $1 = Parameter name
#                  $2 = Path name <dcelocal>
# Return values:   0  = Function successfully performed
#		   1  = Can't get parameter value
	awk '$1 == "'$1'" { for (i = 2; i < NF && index ($(i), "#") != 1; i++)
				printf "%s", $(i)
			    exit
			  }' $2/$CL_PATH/conf/dirparam || return 1
	return 0
}

# ----------------------------------------------------------------------------

send_command() {
# Short description: This function gets information about the active directory
#		     system processes of type (DUA-cache, short-id, DSA)
#		     and sends the given distributed command to these
#		     processes (if there are any).
# Input parameter: $1 = Distributed command code
#		   $2 = Directory identifier
# Return values:   0  = Function successfully performed
#		   8  = Can't send the distributed command
#
    # get information about the active directory system processes
    eval `gdsdirinfo | \
    awk 'BEGIN { NOPROC=0;CAIPCID=0;DSIPCID=0 }
		$1 == "DUA-Cache"         { NOPROC++; CAIPCID=$4 }
		$1 == "DSA" && $3 == '$2' { NOPROC++; DSIPCID=$4 }
		END { print "NOPROC="NOPROC"; IPCIDS="DSIPCID"/"CAIPCID }'` 
    echo "send_command>>: procinfo (NOPROC: $NOPROC IPCIDS: $IPCIDS)" >&2
    if [ "$NOPROC" -gt 0 ]
    then
	# send distributed command to the active directory system processes
	gdsdistcmd -c$1 -s$IPCIDS -t$TIMEOUT >&2 || return 8
    fi
    return 0
}

# ----------------------------------------------------------------------------

map_error() {
# Short description: This function mappes the error which is returned from
#		     fdrestore to the correct error value in the mask (if
#		     restore from file is done).
#		     Restore from diskette or tape doesn't affect any mapping.
# Input parameter: $1 error value from restore
# Return values:   $1 (if Medium != 2)
#		   39 = Can't read file list from file
#		   37 = Can't read data from file

if [ $PAR5 -eq 2 ]
then
	case $1 in
		11) # Can't read file list from media volume
		    return 39;;
		18) # Can't read data from media volume
		    return 37;;
    		*)  # unknown command
		    exit 2
	esac	
else
	return $1
fi
}

# ----------------------------------------------------------------------------

fdrestore () {
# Short description: The function reads all files, described by the distri-
#		     bution file (which is always read first), from the
#		     different media volumes. The files of the first media
#		     volume are only read, if the configuration information
#		     and the user key (contain in the distribution file of
#		     this volume) correspond to the given informations (see
#		     arguments $[3-5]). The files of subsequent media volumes
#		     are only read, if the label and the user key (contained
#		     in the distribution file of the specific volume)
#		     correspond to the label resp. user key contained in the
#		     distribution file of the first media volume.
# Input parameter:   $1 = Path name <dcelocal>
#		     $2 = Base name of the file which contains the 
#			  distribution information
#		     $3 = Directory system identifier
#		     $4 = No. of the volume which should be read
#		     $5 = Device name of the media volume
#		     $6 = User key
#                    $7 = Flags for tar
# Return values:     0  = Function successfully performed
#		     6  = Can't read configuration information
#		     11 = Can't read distribution file from media volume
#		     12 = Wrong label
#		     13 = Wrong user key
#		     15 = Distribution file contains invalid configuration
#		          information
#		     18 = Can't read data from media volume
#		     19 = Can't read distribution file information
#                    26 = Restored data base doesn't agree to the DSA
#                    27 = Restored data base doesn't agree to the DUA-cache
#		     37 = Can't read data from file
#                    38 = File doesn't exist
#		     39 = Can't read file list from file

    cd $1

    # file doesn't exist
    [ $PAR5 -eq 2 -a ! -s $5 ] && return 38

    # read distribution file information from media volume
    tar $7 $5 ./$CL_PATH/${2}$4 >&2 || map_error 11 || return $?

    # read informations from distribution file 1
    eval `awk ' BEGIN { NOFOUND = 0 }
          $1 == ":label:" { $1 = ""; printf "LABEL1=\"%s\"; ", $0; if (++NOFOUND == 3) exit; next }
          $1 == ":dkey:"  { printf "USRKEY1=%s; ", $2; if (++NOFOUND == 3) exit; next }
          $1 == ":cinfo:" { $1 = ""; printf "DIRID1=%d; CTYPE1=%s; DBTYPE1=%s;", \
	  $2, $3, $4; if (++NOFOUND == 3) exit; next } ' $1/$CL_PATH/${2}1` || return 19

    if [ $4 -eq 1 ]
    then
	# first volume - read informations from configuration file
	eval `awk '$2 == '$3' { printf "DIRID=%d; CTYPE=%s; DBTYPE=%s", $2, $4, $10; exit }' \
	$1/etc/gdsconfig` || return 6
	echo "fdrestore>>: DIRID=$DIRID CTYPE=$CTYPE DBTYPE=$DBTYPE" >&2

	# verify distribution information
	[ "$USRKEY1" != "$6" ] && return 13
	[ "$DIRID1" != "$DIRID" -o "$CTYPE1" != "$CTYPE" -o "$DBTYPE1" != "$DBTYPE" ] && return 15
    else
	# read informations from distribution file 
	eval `awk ' BEGIN { NOFOUND = 0 }
	      $1 == ":label:" { $1 = ""; printf "LABELN=\"%s\"; ", $0; if (++NOFOUND == 3) exit; next }
	      $1 == ":dkey:"  { printf "USRKEYN=%s; ", $2; if (++NOFOUND == 3) exit; next }
	      $1 == ":cinfo:" { $1 = ""; printf "DIRIDN=%d; CTYPEN=%s; DBTYPEN=%s;", \
	      $2, $3, $4; if (++NOFOUND == 3) exit; next } ' $1/$CL_PATH/${2}$4` || return 19

	# verify distribution information
	[ "$LABELN" != "$LABEL1" ] && return 12
	[ "$USRKEYN" != "$USRKEY1" ] && return 13
	[ "$DIRIDN" != "$DIRID1" -o "$CTYPEN" != "$CTYPE1" -o "$DBTYPEN" != "$DBTYPE1" ] && return 15
    fi

    # read files from media volume
    tar $7 $5 `awk 'BEGIN { found = 0 }
			$1 ~ /^:/ { next }
			$1 == '$4' { if (++found > 1) printf "%s ", $3; next }
			$1 != '$4' { if (found > 0) exit }' $1/$CL_PATH/${2}1 || return 19` >&2 || map_error 18 || return $?
    return 0
}

# ----------------------------------------------------------------------------

# main () {
    # transfer the values of the positional parameters to the variables PAR1...7
    for i in 1 2 3 4 5 6 7
    do
    	eval PAR$i=$1; shift
    done

    DISDBACCESS=8               # distributed command: disable data base access
    RESTARTPROC=5		# distributed command: restart
    TIMEOUT=60			# distributed command timeout value (in sec)
    DISTFILE="tmp/vol$PAR2."	# distribution file (base name)
    DSADIR=dsa/dir
    CL_PATH=var/adm/directory/gds
    SV_PATH=var/directory/gds
    CRONFILE=$PAR1/$CL_PATH/.crontab
    SLASH=0

    # disable interrupts
    trap "" 1 2 3 15

    # evaluate command code
    case $PAR3 in
    1)	# read command
	if [ $PAR4 -eq 1 ]
	then
	    # verify configuration information
	    eval `awk ' $2 == '$PAR2' { printf "CONF=TRUE"; exit }' $PAR1/etc/gdsconfig` || exit 6
	    [ "$CONF" != "TRUE" ] && exit 6

	    # disable directory system processes
	    send_command $DISDBACCESS $PAR2 || exit $?

	    # remove old distribution information and crontab information
	    rm -f $PAR1/$CL_PATH/${DISTFILE}* $PAR1/$CL_PATH/tmp/crontab.out
	fi

	# read media informations
	if [ $PAR5 -eq 0 ]
	then
	       DEVNAME="`get_dirparam FLOPPYDEVICE $PAR1`"
	else
	    if [ $PAR5 -eq 2 ]
	    then
		# if filename starts with '/' then use absolute path
		eval `echo "$PAR7" | awk '$1 ~ /^\// { printf "SLASH=1"; exit }' `
		if [ $SLASH -eq 1 ]
		then
			DEVNAME=$PAR7
		else
			DEVNAME="`get_dirparam TARPATH $PAR1`"
			DEVNAME=$DEVNAME/$PAR7
		fi
	    else
		DEVNAME="`get_dirparam TAPEDEVICE $PAR1`"
	    fi
	fi
	TARFLAGS="`get_dirparam READFLAGS $PAR1`"
	fdrestore $PAR1 $DISTFILE $PAR2 $PAR4 $DEVNAME "$PAR6" $TARFLAGS || exit $? ;;
    2)	# stop command
	# remove temporary used files
	rm -f $PAR1/$CL_PATH/${DISTFILE}*

	# verify DUA-cache data base versions
	[ "`what $PAR1/$CL_PATH/cache/CACHEDIR$PAR2/cdescr|grep '^[     ]*V.*\.0$'`" != \
	  "`what $PAR1/bin/gdscache|grep '^[    ]*V.*\.0$'`" ] && exit 27

	# Update DSA-name and data base
	if [ `awk ' $2 == '$PAR2' { printf "%s", $4; exit }' $PAR1/etc/gdsconfig ` != "Client-System" ]
	then
	    # verify DSA-data base versions
	    [ "`cat $PAR1/$SV_PATH/dsa/dversion`" != "`cat $PAR1/$SV_PATH/dsa/dir$PAR2/dversion`" ] && exit 26
	    cd $PAR1/$SV_PATH/$DSADIR$PAR2
	    rm -f scheme
	    DBSYSTEM=`awk ' $2 == '$PAR2' { printf "%s", $10; exit }' $PAR1/etc/gdsconfig `
	    INSTALL=$PAR1/$SV_PATH/$DSADIR$PAR2
	    export INSTALL
	    LOGDIR=$PAR1/$SV_PATH/adm/$DSADIR$PAR2
	    export LOGDIR
	    case $DBSYSTEM in
	    C-isam)   gdschdb $INSTALL $LOGDIR ;;
	    informix) inf/d26chdb $INSTALL ;;
	    *)        echo ">>: unknown data base system" >&2
		      exit 29
	    esac
	    if [ $? != 0 ]
	    then
		echo ">>: Generation of local DSA schema impossible !" >&2
		send_command $RESTARTPROC $PAR2 ; exit 29
	    fi
            if [ -s dsaname ]
            then
                DSANAMEOLD=`cat dsaname`
                export DSANAMEOLD
                DSANAME=`cat dsaname.new`
		export DSANAME
		if [ "$DSANAME" != "" -a "$DSANAME" != "$DSANAMEOLD" ]
                then
	    	    # remove invalid crontab information
		    rm -f $PAR1/$CL_PATH/tmp/crontab.out
		    case $DBSYSTEM in
		    C-isam)   gdsmkupd $PAR1/$CL_PATH $PAR2 ;;
		    informix) inf/d26mkupd $PAR1/$CL_PATH $PAR2 ;;
		    esac
                    if [ $? = 1 ]
                    then
			echo ">>: Changing of the DSA-name impossible !" >&2
                        send_command $RESTARTPROC $PAR2 ; exit 29
                    fi
                fi
            else
                mv dsaname.new dsaname
            fi
	fi

        # Update crontab entries
	[ -f $CRONFILE ] && {
		# save global crontab-entries 
		crontab -l|sort > $PAR1/$CL_PATH/.crontab1.$$
		cat $PAR1/$CL_PATH/.crontab|sort > $PAR1/$CL_PATH/.crontab2.$$
		comm -23 $PAR1/$CL_PATH/.crontab1.$$ $PAR1/$CL_PATH/.crontab2.$$ > \
							$PAR1/$CL_PATH/.crontab3.$$
	        # remove old crontab-entries from the cronfile
	        awk '$(NF-1) == '$PAR2' { next };
	        { print $0 }' $CRONFILE > $PAR1/$CL_PATH/tmp/.crontab.$$
	        mv $PAR1/$CL_PATH/tmp/.crontab.$$ $CRONFILE; }
	[ -s $PAR1/$CL_PATH/tmp/crontab.out ] && {
	        # add restored crontab-entries to the cronfile
		cat $PAR1/$CL_PATH/tmp/crontab.out >> $CRONFILE
	        rm $PAR1/$CL_PATH/tmp/crontab.out; }

	[ -f $CRONFILE ] && {
		# global crontab entries available
		cat $CRONFILE >> $PAR1/$CL_PATH/.crontab3.$$

		# any crontab entries available
		if [ -s $PAR1/$CL_PATH/.crontab3.$$ ]
		then
			# Update crontab
			crontab $PAR1/$CL_PATH/.crontab3.$$
		else
			# Remove crontab
			crontab -r
		fi
		rm $PAR1/$CL_PATH/.crontab[1-3].$$; }

	send_command $RESTARTPROC $PAR2 ;;
    *)	# unknown command
	exit 2
    esac
    exit 0
# }
