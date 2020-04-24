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
# $Log: gdsdbwrite.sh,v $
# Revision 1.1.11.2  1996/03/09  23:13:00  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:11  marty]
#
# Revision 1.1.11.1  1995/12/08  15:25:39  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  18:20 UTC  dtruong
# 	Merge to DCE 1.1
# 
# 	HP revision /main/HPDCE01/1  1994/04/19  00:19 UTC  dtruong
# 	Port to HPUX 10.0
# 	[1995/12/08  14:54:03  root]
# 
# Revision 1.1.7.2  1993/08/11  14:19:41  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:10:44  marrek]
# 
# Revision 1.1.5.6  1993/02/02  10:11:00  marrek
# 	use absolut path if filename starts with '/'
# 	[1993/02/01  17:55:26  marrek]
# 
# Revision 1.1.5.5  1993/01/29  08:39:24  marrek
# 	new parameter for get_dirparam
# 	[1993/01/28  10:45:41  marrek]
# 
# Revision 1.1.5.4  1992/12/31  18:39:15  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:11:51  bbelch]
# 
# Revision 1.1.5.3  1992/11/27  17:01:13  marrek
# 	November 1992 code drop
# 	[1992/11/17  14:55:24  marrek]
# 
# Revision 1.1.5.2  1992/10/05  16:43:44  marrek
# 	Insert #!/bin/sh .
# 	[1992/10/05  16:40:57  marrek]
# 
# Revision 1.1.2.3  1992/06/30  20:55:15  melman
# 	GDS drop of 6/16/92
# 	[1992/06/30  20:34:21  melman]
# 
# Revision 1.1.2.2  1992/06/01  20:10:23  melman
# 	New GDS merged drop
# 	[1992/05/28  21:04:31  melman]
# 
# $EndLog$
#
# $Id: gdsdbwrite.sh,v 1.1.11.2 1996/03/09 23:13:00 marty Exp $
#
#****************************************************************************
#                                                                           *
#         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
#                                ALL RIGHTS RESERVED                        *
#                                                                           *
#****************************************************************************

# Shell-script used to save directory system data informations
# Input parameter:
#               $1 = Path name <dcelocal>
#		$2 = Directory system identifier
#		$3 = Command code
#		     1 = Start operation
#		     2 = Write file(s) to media
#		     3 = Stop operation
#		$4 = User key (if command code = 1)
#		     Volume no (if command code = 2)
#		     (irrelevant otherwise)
#		$5 = Medium (0 = floppy disk, 1 = streamer tape, 2 = file)
#		     (if command code = 1 or 2)
#		     (irrelevant otherwise)
#		$6 = Format media (yes/no) (if command code = 2)
#		     (irrelevant otherwise)
#		$7 = Filename (to which shall be saved)
#		     (if medium = 2 - irrelevant otherwise)
# Exit values:
#		0  = Operation successfully performed
#		2  = The function can't be executed
#		6  = Configuration data not found	
#		8  = Can't send distributed command
#		16 = Can't format media volume
#		17 = Can't write to media volume
#		19 = Can't read file list
#               21 = Can't save data, because one file exceeds the media
#		     volume size
#               22 = No files found which can be saved
#		36 = Can't write data to file

# ----------------------------------------------------------------------------

get_dirparam() {
# Short description: This function gets the value which is assigned to the given
#		     parameter (name) from the directory system parameter file.
# Input parameter: $1 = Parameter name
#                  $2 = Path name <dcelocal>
#                  $3 = blank shall be appended to output (yes/no)
# Return values:   0  = Function successfully performed
#		   1  = Can't get parameter value
	case $3 in
	no )  # don't append blank to output
	      awk '$1 == "'$1'" { for (i = 2; i < NF && index ($(i), "#") != 1; i++)
				printf "%s", $(i)
			        exit
			  }' $2/$CL_PATH/conf/dirparam || return 1;;
	yes ) # append blank to output
	      awk '$1 == "'$1'" { for (i = 2; i < NF && index ($(i), "#") != 1; i++)
				printf "%s ", $(i)
			        exit
			  }' $2/$CL_PATH/conf/dirparam || return 1;;
	* )   # wrong input
	      return 1;;
	esac

	return 0
}

# ----------------------------------------------------------------------------

send_command() {
# Short description: This function gets information about the active directory
#		     system processes of type 'DUA-cache, short-id and DSA'
#		     and sends the given distributed command to these
#		     processes (if there are any).
# Input parameter: $1 = Distributed command code
#		   $2 = Directory identifier
# Return values:   0  = Function successfully performed
#		   8  = Can't send distributed command
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
#		     fdsave to the correct error value in the mask (if save 
#		     to file is done).
#		     Save to diskette or tape doesn't affect any mapping.
# Input parameter: $1 error value from restore
# Return values:   $1 (if Medium != 2)
#		   36 = Can't write to file

if [ $PAR5 -eq 2 ]
then
	case $1 in
		17) # Can't write data to media volume
		    return 36;;
    		*)  # unknown command
		    exit 2
	esac	
else
	return $1
fi
}

# ----------------------------------------------------------------------------

fdfit() {
# Short description: The function tries to arrange a number of files
#		     onto the fewest number of media volumes while avoiding
#		     splitting any file across two or more media volumes.
#		     The arrangement, if found any, is written to the given
#		     distribution file.
#		     Note: The function must be called by using the shell
#			   'eval'-command.
# Input parameter:   $1 = Name(s) of the file(s) which should arranged
#		     $2 = Volume size of one media volume (no. of blocks)
#		     $3 = Block size of the media volume (no. of bytes)
#		     $4 = Name of the distribution file(s)
#		     $5 = User key name
#		     $6 = Configuration information
#                    $7 = Path name <dcelocal>
# Return values:     ERRNO=0  - Operation successfully performed
#                    ERRNO=21 - No arrangement found, because one
#		 		file exceeds the media volume size
#                    ERRNO=22 - No files found

    # used constants: 
    ERRNO1=21   # error (file too big)
    ERRNO2=22   # error (no files found)

    gdsutil -s $1 | sort +0n -1 | \
    awk ' BEGIN { NOFILES=0; I=0; ERRNO=0 }
        {
          FBLOCKS[I]  = int (($1/BLOCKSIZE)+1) ;
          FSIZE[I]    = $1 ;
          FNAME[I]    = $2 ;
          FVOLUME[I]  = 0 ;
          FHANDLED[I] = 0 ;
          if (FBLOCKS[I] > int (VOLUMESIZE)) {
              printf "ERRNO='$ERRNO1'; " ;
              ERRNO=1;
              exit ;
          }
          NOFILES++ ; I++ ;
        }
        END {
          if (ERRNO != 0) exit ;
          if (NOFILES == 0) {
              printf "ERRNO='$ERRNO2'; " ;
              exit ;
          }
          I=NOFILES-1 ; VOLUMENO=1 ;
          USEDBLOCKS=int (((NOFILES * 80)/BLOCKSIZE)+1) ;
          MAXINDEX=I ; LASTENTRY=1 ; NOFILESS=NOFILES ;
          while (NOFILES > 0) {
              if (FHANDLED[I] == 0) {
		  if (USEDBLOCKS + FBLOCKS[I] <= int (VOLUMESIZE)) {
		      # file fits onto the actual media volume
	              USEDBLOCKS += FBLOCKS[I] ;
		      FVOLUME[I] = VOLUMENO ;
		      FHANDLED[I] = 1 ;
		      NOFILES-- ;
		      if (LASTENTRY == 1)
		          MAXINDEX-- ;
                  } else
		      LASTENTRY = 0 ;
              }
              if (I == 0 && NOFILES > 0) {
                  # go to next media volume
	          VOLUMENO++ ;
	          USEDBLOCKS = 1 ;
	          I = MAXINDEX ;
	          LASTENTRY = 1 ;
              } else
	          I-- ;
          }
          # write arrangement to the distribution file(s)
          for (I = 1; I <= VOLUMENO; I++) {
              printf "echo \":label:\\t%s\" > %s/%s%d; ", LABEL, DIRPATH, DFILE, I
    	      printf "echo \":dkey:\\t%s\" >> %s/%s%d; ", USRKEY, DIRPATH, DFILE, I
              printf "echo \":cinfo:\\t%s\" >> %s/%s%d; ", CINFO, DIRPATH, DFILE, I
          }
          printf "echo \":nvol:\\t%d\" >> %s/%s1; ", VOLUMENO, DIRPATH, DFILE
          for (I = 1; I <= VOLUMENO; I++) {
	      printf "echo \"%d\\t0\\t%s/%s%d\" >> %s/%s1; ", I, DIRPATH, DFILE, I, DIRPATH, DFILE
	      for (J = 0; J < NOFILESS; J++) {
	          if (FVOLUME[J] == I)
		      printf "echo \"%d\\t%d\\t%s\" >> %s/%s1; ", I, FSIZE[J], FNAME[J], DIRPATH, DFILE
	      }
          }
          printf "ERRNO=0; "
	} ' VOLUMESIZE=$2 BLOCKSIZE=$3 DFILE=$4 USRKEY=${5:--} CINFO="$6" LABEL="`date '+%m/%d/%y %H:%M:%S'`" DIRPATH=./$CL_PATH -
}

# ----------------------------------------------------------------------------

getfdinfo () {
# Short description: The function gets the configuration information of
#		     the given directory identifier from the configuration
#		     file and then defines the files which must be saved.
# Input parameter:   $1 = Path name <dcelocal>
#		     $2 = Directory identifier
# Output parameter:  The Variable DFILES contains the names of all files
#		     which must be saved (only if the return value is 0)
# Return values:     0 = Function successfully performed
#		     6 = No configuration information available
#
    cd $1
    DIRID=0
    eval `awk '$2 == '$2' { printf "DIRID=%d; CTYPE=%s; UPDATE=%s; DBTYPE=%s", $2, $4, $8, $10; exit }' $1/etc/gdsconfig` || return 6
    echo "getfdinfo>>: DIRID=$DIRID CTYPE=$CTYPE UPDATE=$UPDATE DBTYPE=$DBTYPE" >&2

    [ $DIRID -eq 0 ] && return 6

    DFILES=
    # cache data files must be saved
    [ "$CTYPE" != "Clt/Srv-System/L" ] && {
	[ -f ./$CL_PATH/cache/CACHEDIR$2/cattr ] && DFILES="./$CL_PATH/cache/CACHEDIR$2/c*"
    }

    cd $1
    if [ "$CTYPE" != "Client-System" ]
    then
	# common dsa data files must be saved
	[ -s ./$SV_PATH/dsa/dir$2/dsaname ] && {
		cp ./$SV_PATH/dsa/dir$2/dsaname ./$SV_PATH/dsa/dir$2/dsaname.new
		DFILES="$DFILES ./$SV_PATH/dsa/dir$2/dsaname.new"
	}
	DFILES="$DFILES ./$SV_PATH/dsa/dir$2/dversion ./$SV_PATH/dsa/dir$2/dnlist*"

	# delta update files must be saved
	[ "$UPDATE" != "-" ] && {
		[ -f ./$SV_PATH/dsa/dir$2/update/LOCKJOBS ] && DFILES="$DFILES ./$SV_PATH/dsa/dir$2/update/[!log]*"
	}

	if [ "$DBTYPE" = "C-isam" ]
	then
	    # dsa isam data files must be saved
	    DFILES="$DFILES ./$SV_PATH/dsa/dir$2/attr* ./$SV_PATH/dsa/dir$2/objects*"
	else
	    # dsa informix data files must be saved
	    DFILES="$DFILES ./$SV_PATH/dsa/dir$2/dir.dbs/*"
	fi
    fi

    # crontab-entries must be saved
    [ -s $1/$CL_PATH/tmp/crontab.out ] && rm $1/$CL_PATH/tmp/crontab.out
    [ -s $1/$CL_PATH/.crontab ] && \
    awk '$(NF-1) == '$2' { print $0 }' \
         $1/$CL_PATH/.crontab > $1/$CL_PATH/tmp/crontab.out
    [ -s $1/$CL_PATH/tmp/crontab.out ] && DFILES="$DFILES ./$CL_PATH/tmp/crontab.out"

    echo "getfdinfo>>: DFILES=$DFILES" >&2
    return 0
}

# ----------------------------------------------------------------------------

fdsave () {
# short description: The function writes all files, which are described by 
#		     the distribution file list, to the given media volume.
# Input parameter:   $1 = Path name <dcelocal>
#		     $2 = Name of the file which contains the 
#			  distribution file list
#		     $3 = No. of the volume to which the files must be written
#		     $4 = Device name of the media volume(s)
#		     $5 = Format media volume before writing (yes, no)
#                    $6 = Flags for tar
# Return values:     0  = Function successfully performed
#		     16 = Can't format media volume
#		     17 = Can't write data to media volume
#		     19 = Can't get file information from file list
#		     36 = Can't write data to file

    # format media volume if requested
    [ "$5" = "yes" ] && { eval `get_dirparam FLOPPYFORMCMD $1 yes` || return 16; }

    # write files to the media volume
    cd $1
    [ $PAR5 -eq 2 -a ! -d `dirname $4` ] && {
	echo "fdsave>>: Can't write to file $4" >&2
	return 36
    }
    tar $6 $4 `awk 'BEGIN { found = 0 }
		   $1 ~ /^:/ { next }
		   $1 == '$3' { printf "%s ", $3; found = 1; next }
		   $1 != '$3' { if (found == 1) exit }' $1/$CL_PATH/$2 || return 19` >&2 || map_error 17 || return $?
    return 0
}

# ----------------------------------------------------------------------------

# main () {
    # transfer the values of the positional parameters to the variables PAR1...7
    for i in 1 2 3 4 5 6 7
    do
    	eval PAR$i=$1; shift
    done

    DISDBACCESS=3		# distributed command: disable data base access
    ENADBACCESS=4		# distributed command: enable data base access
    TIMEOUT=30			# distributed command time out value (in sec)
    DISTFILE="tmp/vol$PAR2."	# distribution file (base name)
    CL_PATH=var/adm/directory/gds
    SV_PATH=var/directory/gds
    SLASH=0

    # disable interrupts
    trap "" 1 2 3 15

    # evaluate command code
    case $PAR3 in
    1)  # start command
	# disable directory system processes
	send_command $DISDBACCESS $PAR2 || exit $?

	# get names of all files which must be saved
	getfdinfo $PAR1 $PAR2 || exit $?

	# evaluate media type (set volume size and volume block size)
	if [ $PAR5 -eq 0 ]
	then
	    VOLSIZE="`get_dirparam FLOPPYVOLSIZE $PAR1 no`"
	    BLOCKSIZE="`get_dirparam FLOPPYBLSIZE $PAR1 no`"
	else
	    if [ $PAR5 -eq 2 ]
	    then
		VOLSIZE="`get_dirparam FILEVOLSIZE $PAR1 no`"
		BLOCKSIZE="1"
	    else
		VOLSIZE="`get_dirparam TAPEVOLSIZE $PAR1 no`"
		BLOCKSIZE="`get_dirparam TAPEBLSIZE $PAR1 no`"
	    fi
	fi
	# create file list (distribution information file(s))
	eval `fdfit "$DFILES" $VOLSIZE $BLOCKSIZE $DISTFILE "$PAR4" "$DIRID $CTYPE $DBTYPE" $PAR1`
	[ "$ERRNO" -ne 0 ] && { echo "fdfit>>: return value = $ERRNO" >&2; exit $ERRNO; } ;;
    2)  # write file(s) command
	# evaluate media type (set media device name)
	if [ $PAR5 -eq 0 ]
	then
	    DEVNAME="`get_dirparam FLOPPYDEVICE $PAR1 no`"
	    FORMAT=$PAR6
	else
	    if [ $PAR5 -eq 2 ]
	    then
		# if filename starts with '/' then use absolute path
		eval `echo "$PAR7" | awk '$1 ~ /^\// { printf "SLASH=1"; exit }' `
		if [ $SLASH -eq 1 ]
		then
			DEVNAME=$PAR7
		else
			DEVNAME="`get_dirparam TARPATH $PAR1 no`"
			DEVNAME=$DEVNAME/$PAR7
		fi
	    else
		DEVNAME="`get_dirparam TAPEDEVICE $PAR1 no`"
	    fi
	    FORMAT=no
	fi
	TARFLAGS="`get_dirparam WRITEFLAGS $PAR1 no`"

	# write file(s) to media volume
	fdsave $PAR1 ${DISTFILE}1 $PAR4 $DEVNAME $FORMAT $TARFLAGS || exit $? ;;
    3)  # stop command
	# remove temporary used files
	rm -f $PAR1/$CL_PATH/${DISTFILE}* \
	$PAR1/$SV_PATH/dsa/dir$PAR2/dsaname.new \
	$PAR1/$CL_PATH/tmp/crontab.out

	# enable directory system processes
	send_command $ENADBACCESS $PAR2 || exit $? ;;
    *)  # unknown command
	exit 2
    esac

    exit 0
# }
